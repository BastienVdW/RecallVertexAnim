// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "RecallVertexAnimInstanceProcessors.h"

#include "Actor/RecallVertexAnimActor.h"
#include "MassExecutionContext.h"
#include "Simulation/Ability/RecallAbilityFragments.h"
#include "Simulation/Representation/RecallVertexAnimFragments.h"
#include "Simulation/Transform/RecallTransformFragments.h"
#include "System/Actor/RecallActorSubsystem.h"

//----------------------------------------------------------------------//
// URecallVertexAnimInstanceInitializer
//----------------------------------------------------------------------//
URecallVertexAnimInstanceInitializer::URecallVertexAnimInstanceInitializer()
	: EntityQuery(*this)
{
	ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::All);
	ObservedType = FRecallVertexAnimInstanceFragment::StaticStruct();
	Operation = EMassObservedOperation::Add;
}

void URecallVertexAnimInstanceInitializer::InitializeInternal(UObject& Owner, const TSharedRef<FMassEntityManager>& InEntityManager)
{
	Super::InitializeInternal(Owner, InEntityManager);
}

void URecallVertexAnimInstanceInitializer::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager) 
{
	EntityQuery.AddRequirement<FRecallVertexAnimInstanceFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSharedRequirement<FRecallVertexAnimInstanceSharedFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddConstSharedRequirement<FRecallVertexAnimConstSharedFragment>(EMassFragmentPresence::All);
	EntityQuery.AddSubsystemRequirement<URecallActorSubsystem>(EMassFragmentAccess::ReadWrite);
}

void URecallVertexAnimInstanceInitializer::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(Context, [](FMassExecutionContext& Context)
	{
		URecallActorSubsystem& ActorSystem = Context.GetMutableSubsystemChecked<URecallActorSubsystem>();

		FRecallVertexAnimInstanceSharedFragment& InstanceSharedFragment = Context.GetMutableSharedFragment<FRecallVertexAnimInstanceSharedFragment>();

		if (InstanceSharedFragment.InstanceHandle.IsSet() || InstanceSharedFragment.InstanceName.IsNone())
		{
			return;
		}
		
		const FRecallVertexAnimConstSharedFragment& VertexAnimConstSharedFragment = Context.GetConstSharedFragment<FRecallVertexAnimConstSharedFragment>();
		InstanceSharedFragment.InstanceHandle = ActorSystem.CreateActor(VertexAnimConstSharedFragment.Definition);
	});
}

//----------------------------------------------------------------------//
// URecallVertexAnimInstanceRepresentationProcessor
//----------------------------------------------------------------------//
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
static bool bDebugShowVAInfo = false;
static FAutoConsoleVariableRef CVarRecallShowVAInfo(
	TEXT("recall.VA.ShowInfo"),
	bDebugShowVAInfo,
	TEXT("Show VA Info")
);
#endif // UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT

URecallVertexAnimInstanceRepresentationProcessor::URecallVertexAnimInstanceRepresentationProcessor()
	: EntityQuery(*this)
{
	ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::All);
	ProcessingPhase = EMassProcessingPhase::Render;
	bRequiresGameThreadExecution = true;
}

struct FRecallVertexAnimSharedInstanceCache
{
	TWeakObjectPtr<ARecallVertexAnimActor> Actor;
	TArray<FRecallVertexAnimInstance, TFixedAllocator<MS_VA_INSTANCE_MAX>> Instances;
	FCriticalSection DataGuard;

	FORCEINLINE void ResetCache()
	{
		Actor.Reset();
		Instances.Reset();
	}
};

struct FRecallVertexAnimInstanceRepresentationCacheManager
{
	TMap<FName, int32> SharedInstanceToIndex;
	int32 SharedInstanceCount = 0;
	TArray<FRecallVertexAnimSharedInstanceCache> SharedInstances;

	FORCEINLINE void ResetCache()
	{
		SharedInstanceToIndex.Reset();
		
		for (int32 SharedInstanceIndex = 0; SharedInstanceIndex < SharedInstanceCount; SharedInstanceIndex++)
		{
			SharedInstances[SharedInstanceIndex].ResetCache();
		}
		SharedInstanceCount = 0;
	}
};

void URecallVertexAnimInstanceRepresentationProcessor::InitializeInternal(UObject& Owner, const TSharedRef<FMassEntityManager>& InEntityManager)
{
	Super::InitializeInternal(Owner, InEntityManager);

	CacheManager = MakeShared<FRecallVertexAnimInstanceRepresentationCacheManager>();
}

void URecallVertexAnimInstanceRepresentationProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager) 
{
	EntityQuery.AddRequirement<FRecallTransformFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FRecallVertexAnimInstanceFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FRecallAbilityFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddSharedRequirement<FRecallVertexAnimInstanceSharedFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddConstSharedRequirement<FRecallVertexAnimConstSharedFragment>(EMassFragmentPresence::All);
	
	ProcessorRequirements.AddSubsystemRequirement<URecallActorSubsystem>(EMassFragmentAccess::ReadOnly);
}

void URecallVertexAnimInstanceRepresentationProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	QUICK_SCOPE_CYCLE_COUNTER(Recall_VertexAnimInstance_Representation);

	check(CacheManager.IsValid());
	CacheManager->ResetCache();

	TMap<FName, int32>& SharedInstanceToIndex = CacheManager->SharedInstanceToIndex;
	TArray<FRecallVertexAnimSharedInstanceCache>& SharedInstances = CacheManager->SharedInstances;
	int32& SharedInstanceCount = CacheManager->SharedInstanceCount;
	
	const URecallActorSubsystem& ActorSystem = Context.GetSubsystemChecked<URecallActorSubsystem>();

	Context.GetEntityManagerChecked().ForEachSharedFragment<FRecallVertexAnimInstanceSharedFragment>(
		[&ActorSystem, &SharedInstanceToIndex, &SharedInstanceCount, &SharedInstances](FRecallVertexAnimInstanceSharedFragment& InstanceSharedFragment)
		{
			if (SharedInstanceToIndex.Contains(InstanceSharedFragment.InstanceName))
			{
				return;
			}

			SharedInstanceToIndex.Add(InstanceSharedFragment.InstanceName, SharedInstanceCount);

			if (SharedInstanceCount == SharedInstances.Num())
			{
				SharedInstances.AddDefaulted();
			}
			
			FRecallVertexAnimSharedInstanceCache& SharedInstance = SharedInstances[SharedInstanceCount++];
			SharedInstance.Actor = ActorSystem.GetActor<ARecallVertexAnimActor>(InstanceSharedFragment.InstanceHandle);
		}
	);

	EntityQuery.ParallelForEachEntityChunk(Context,
		[&SharedInstanceToIndex, &SharedInstances](FMassExecutionContext& Context)
	{
		const auto& InstanceSharedFragment = Context.GetSharedFragment<FRecallVertexAnimInstanceSharedFragment>();
		const int32 InstanceIndex = SharedInstanceToIndex.FindChecked(InstanceSharedFragment.InstanceName);
		FRecallVertexAnimSharedInstanceCache& SharedInstance = SharedInstances[InstanceIndex];

		int32 StartIndex = 0;
		{
			FScopeLock Lock(&SharedInstance.DataGuard);
			StartIndex = SharedInstance.Instances.Num();
			SharedInstance.Instances.AddDefaulted(Context.GetNumEntities());
		}
			
		const TConstArrayView<FRecallVertexAnimInstanceFragment> VertexAnimInstanceList = Context.GetFragmentView<FRecallVertexAnimInstanceFragment>();
		const TConstArrayView<FRecallTransformFragment> TransformList = Context.GetFragmentView<FRecallTransformFragment>();
		const TConstArrayView<FRecallAbilityFragment> AbilityList = Context.GetFragmentView<FRecallAbilityFragment>();
		
		ParallelFor(Context.GetNumEntities(), [&](int32 EntityIndex)
		{
			const FRecallVertexAnimInstanceFragment& VertexAnimInstanceFragment = VertexAnimInstanceList[EntityIndex];
			const FRecallTransformFragment& TransformFragment = TransformList[EntityIndex];
			const FRecallAbilityFragment& AbilityFragment = AbilityList[EntityIndex];		
			
			FRecallVertexAnimInstance& Instance = SharedInstance.Instances[StartIndex + EntityIndex];
			Instance.Ability = AbilityFragment.CurrentAbility;
			Instance.Frame = AbilityFragment.CurrentFrameTime;
			Instance.Transform = VertexAnimInstanceFragment.Offset * TransformFragment.GetTransform();
		});
	});

	for (const FRecallVertexAnimSharedInstanceCache& SharedInstance : SharedInstances)
	{
		if (!SharedInstance.Actor.IsValid())
		{
			continue;
		}

		SharedInstance.Actor->BatchSetInstances(SharedInstance.Instances);
	}

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	if (bDebugShowVAInfo)
	{
		FString DebugString;
		DebugString += FString::Printf(TEXT("VA Shared Instance Count: %d\n"), SharedInstances.Num());

		for (const TTuple<FName, int32>& SharedInstanceToIdx : SharedInstanceToIndex)
		{
			const FRecallVertexAnimSharedInstanceCache& SharedInstance = SharedInstances[SharedInstanceToIdx.Value];
			DebugString += FString::Printf(TEXT("* %s: %d\n"),
				*SharedInstanceToIdx.Key.ToString(), SharedInstance.Instances.Num());
		}
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugString);
		}
	}
#endif // UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
}
