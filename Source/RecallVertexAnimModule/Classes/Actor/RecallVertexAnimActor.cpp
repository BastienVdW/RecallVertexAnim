// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "RecallVertexAnimActor.h"

#include "AnimToTextureDataAsset.h"
#include "AnimToTextureInstancePlaybackHelpers.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Data/Ability/RecallAbilityAsset.h"
#include "Data/VertexAnim/RecallVertexAnimAsset.h"

ARecallVertexAnimActor::ARecallVertexAnimActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancedMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMeshComponent"));
	SetRootComponent(InstancedMeshComponent);
}

void ARecallVertexAnimActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetInstanceCount(DefaultInstanceCount);
}

void ARecallVertexAnimActor::SetInstanceCount(int32 Count)
{
	check(Count >= 0);
	if (InstanceCount == Count)
	{
		return;
	}

	InstanceCount = Count;
	UAnimToTextureInstancePlaybackLibrary::SetupInstancedMeshComponent(InstancedMeshComponent, InstanceCount, bAutoPlayData);
	
	BatchData.SetNum(InstanceCount);
	BatchInstanceData.SetNumUninitialized(InstanceCount);
	BatchCustomData.SetNumUninitialized(BatchData.GetTypeSize() / sizeof(float) * InstanceCount);
}

bool ARecallVertexAnimActor::FindFrameData(const URecallAbilityAsset* Ability,
	float Frame, FAnimToTextureFrameData& OutFrameData)
{
	if (!Ability || Ability->AnimationSections.Num() == 0)
	{
		return false;
	}

	const FRecallAbilityAnimationSection* WorkingSection = nullptr;
	const float SectionStartFrame = Ability->GetSectionStartFrame(Frame, WorkingSection);
	if (WorkingSection->Duration == 0)
	{
		return false;
	}
	
	// If there is a valid animation, play it.
	const TObjectPtr<URecallVertexAnimAsset> VertexAnimAsset = Cast<URecallVertexAnimAsset>(
		WorkingSection->GetAnimationAsset());
	if (!VertexAnimAsset)
	{
		return false;
	}

	const UAnimToTextureDataAsset* AnimToTextureDataAsset = VertexAnimAsset->GetAnimToTextureDataAsset();
	if (!AnimToTextureDataAsset)
	{
		return false;
	}
	
	// Sample rate delta time.
	const float SampleRate = AnimToTextureDataAsset->SampleRate;
	const float FixedDeltaTime = 1.0f / SampleRate;
	
	// Section animation duration.
	const float AnimStartFrame =  WorkingSection->AnimFrameRange.GetLowerBoundValue();
	const float AnimEndFrame =  WorkingSection->AnimFrameRange.GetUpperBoundValue();
	const float SectionAnimDuration = AnimEndFrame - AnimStartFrame + 1.0f;	
	
	// Calculate and set the animation offset.
	const float PlayRate = SectionAnimDuration / WorkingSection->Duration;
	const float SectionElapsed = Frame - SectionStartFrame;
	const float Time = (SectionElapsed * FixedDeltaTime) * PlayRate;	
	const float TimeOffset = AnimStartFrame * FixedDeltaTime;
	const bool bMotionBlur = SectionAnimDuration > 1.0f;
	
	const bool bFound = UAnimToTextureInstancePlaybackLibrary::GetFrameDataFromDataAsset(
		AnimToTextureDataAsset, VertexAnimAsset->GetAnimationIndex(), Time, OutFrameData, TimeOffset, 1.0f);
	
	if (!bMotionBlur)
	{
		OutFrameData.PrevFrame = OutFrameData.Frame;
	}

	return bFound;
}

void ARecallVertexAnimActor::SynchronizeToAbilityFrame(const URecallAbilityAsset* Ability,
                                                         float Frame, int32 InstanceIndex /*= 0*/) const
{
	FAnimToTextureFrameData FrameData;
	if (FindFrameData(Ability, Frame, FrameData))
	{
		UAnimToTextureInstancePlaybackLibrary::UpdateInstanceFrameData(
			InstancedMeshComponent, InstanceIndex, FrameData);
	}
}

void ARecallVertexAnimActor::BatchSetInstances(const TArray<FRecallVertexAnimInstance, TFixedAllocator<MS_VA_INSTANCE_MAX>>& Instances)
{
	TRACE_CPUPROFILER_EVENT_SCOPE_STR("ARecallVertexAnimActor::BatchSetInstances");
	
	if (bUpdatingInstancedMesh)
	{
		return;
	}
		
	bUpdatingInstancedMesh = true;
	
	SetInstanceCount(Instances.Num());

	TWeakObjectPtr<ARecallVertexAnimActor> WeakSelf = this;
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [WeakSelf, Instances]()
	{
		TRACE_CPUPROFILER_EVENT_SCOPE_STR("ARecallVertexAnimActor::BatchSetInstances FindFrameData");
		if (!WeakSelf.IsValid())
		{
			return;
		}
				
		const int32 NumInstances = Instances.Num();
		
		TArray<FInstancedStaticMeshInstanceData>& BatchInstanceData = WeakSelf->BatchInstanceData;
		TArray<FAnimToTextureFrameData>& BatchData = WeakSelf->BatchData;
		
		ParallelFor(NumInstances, [&Instances, &BatchInstanceData, &BatchData](int32 InstanceIndex)
		{
			const FRecallVertexAnimInstance& Instance = Instances[InstanceIndex];
			BatchInstanceData[InstanceIndex] = Instance.Transform.ToMatrixNoScale();
			FindFrameData(Instance.Ability, Instance.Frame, BatchData[InstanceIndex]);
		});

		TArray<float>& BatchCustomData = WeakSelf->BatchCustomData;
		
		FMemory::Memcpy(BatchCustomData.GetData(), BatchData.GetData(), BatchData.GetTypeSize() * NumInstances);

		// Batch Update
		AsyncTask(ENamedThreads::GameThread, [WeakSelf]()
		{
			TRACE_CPUPROFILER_EVENT_SCOPE_STR("ARecallVertexAnimActor::BatchSetInstances BatchUpdateInstancesData");
			if (WeakSelf.IsValid())
			{
				WeakSelf->InstancedMeshComponent->BatchUpdateInstancesData(0, WeakSelf->InstanceCount,
					WeakSelf->BatchInstanceData.GetData(), true, false);
				WeakSelf->InstancedMeshComponent->SetCustomData(0, WeakSelf->InstanceCount - 1,
					WeakSelf->BatchCustomData, true);
				WeakSelf->bUpdatingInstancedMesh = false;
			}			
		});
	});
}
