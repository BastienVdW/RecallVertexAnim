// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "RecallVertexAnimProcessors.h"

#include "Actor/RecallVertexAnimActor.h"
#include "MassExecutionContext.h"
#include "Simulation/Ability/RecallAbilityFragments.h"
#include "Simulation/Representation/RecallVertexAnimFragments.h"
#include "Simulation/Representation/RecallActorRepresentationFragments.h"
#include "System/Actor/RecallActorSubsystem.h"

//----------------------------------------------------------------------//
// URecallVertexAnimInitializer
//----------------------------------------------------------------------//
URecallVertexAnimInitializer::URecallVertexAnimInitializer()
	: EntityQuery(*this)
{
	ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::All);
	ObservedType = FRecallActorRepresentationFragment::StaticStruct();
	Operation = EMassObservedOperation::Add;
}

void URecallVertexAnimInitializer::InitializeInternal(UObject& Owner, const TSharedRef<FMassEntityManager>& InEntityManager)
{
	Super::InitializeInternal(Owner, InEntityManager);
}

void URecallVertexAnimInitializer::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager) 
{
	EntityQuery.AddRequirement<FRecallActorRepresentationFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddConstSharedRequirement<FRecallVertexAnimConstSharedFragment>(EMassFragmentPresence::All);
	EntityQuery.AddSubsystemRequirement<URecallActorSubsystem>(EMassFragmentAccess::ReadWrite);
}

void URecallVertexAnimInitializer::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(Context, [](FMassExecutionContext& Context)
	{
		URecallActorSubsystem& ActorSystem = Context.GetMutableSubsystemChecked<URecallActorSubsystem>();

		const FRecallVertexAnimConstSharedFragment& VertexAnimConstSharedFragment = Context.GetConstSharedFragment<FRecallVertexAnimConstSharedFragment>();

		const TArrayView<FRecallActorRepresentationFragment> ActorList = Context.GetMutableFragmentView<FRecallActorRepresentationFragment>();

		for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); EntityIndex++)
		{
			FRecallActorRepresentationFragment& ActorFragment = ActorList[EntityIndex];
			if (ActorFragment.ActorHandle.IsSet())
			{
				continue;
			}

			ActorFragment.ActorHandle = ActorSystem.CreateActor(VertexAnimConstSharedFragment.Definition);
		}
	});
}

//----------------------------------------------------------------------//
// URecallAbilityAnimationVertexAnimRepresentationProcessor
//----------------------------------------------------------------------//
URecallAbilityAnimationVertexAnimRepresentationProcessor::URecallAbilityAnimationVertexAnimRepresentationProcessor()
	: EntityQuery(*this)
{
	ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::All);
	ProcessingPhase = EMassProcessingPhase::Render;
	bRequiresGameThreadExecution = true;
}

void URecallAbilityAnimationVertexAnimRepresentationProcessor::InitializeInternal(UObject& Owner, const TSharedRef<FMassEntityManager>& InEntityManager)
{
	Super::InitializeInternal(Owner, InEntityManager);
}

void URecallAbilityAnimationVertexAnimRepresentationProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager) 
{
	EntityQuery.AddRequirement<FRecallActorRepresentationFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FRecallAbilityFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddConstSharedRequirement<FRecallVertexAnimConstSharedFragment>(EMassFragmentPresence::All);
	EntityQuery.AddSubsystemRequirement<URecallActorSubsystem>(EMassFragmentAccess::ReadOnly);
}

void URecallAbilityAnimationVertexAnimRepresentationProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	QUICK_SCOPE_CYCLE_COUNTER(Recall_AbilityAnimationVertexAnim_Representation);

	EntityQuery.ForEachEntityChunk(Context, [](FMassExecutionContext& Context)
	{
		const URecallActorSubsystem& ActorSystem = Context.GetSubsystemChecked<URecallActorSubsystem>();
		
		const TConstArrayView<FRecallAbilityFragment> AbilityList = Context.GetFragmentView<FRecallAbilityFragment>();
			
		const TConstArrayView<FRecallActorRepresentationFragment> ActorList = Context.GetFragmentView<FRecallActorRepresentationFragment>();
		
		for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); EntityIndex++)
		{	
			const FRecallActorRepresentationFragment& ActorFragment = ActorList[EntityIndex];
			
			const TWeakObjectPtr<ARecallVertexAnimActor> Actor = ActorSystem.GetActor<ARecallVertexAnimActor>(ActorFragment.ActorHandle);
			if (!Actor.IsValid())
			{
				continue;
			}

			const FRecallAbilityFragment& AbilityFragment = AbilityList[EntityIndex];			
			Actor->SynchronizeToAbilityFrame(AbilityFragment.CurrentAbility, AbilityFragment.CurrentFrameTime);
		}
	});
}
