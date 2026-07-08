// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "RecallVertexAnimAsset.h"

#include "AnimToTextureDataAsset.h"

int32 URecallVertexAnimAsset::GetAnimationIndex() const
{
	if (AnimToTextureDataAsset)
	{
		for (int32 AnimIndex = 0; AnimIndex < AnimToTextureDataAsset->AnimSequences.Num(); AnimIndex++)
		{
			const FAnimToTextureAnimSequenceInfo& AnimSequenceInfo = AnimToTextureDataAsset->AnimSequences[AnimIndex];
			if (AnimSequenceInfo.AnimSequence && AnimSequenceInfo.AnimSequence->GetFName() == AnimationName)
			{
				return AnimIndex;
			}
		}
	}

	return INDEX_NONE;
}

const UAnimToTextureDataAsset* URecallVertexAnimAsset::GetAnimToTextureDataAsset() const
{
	return AnimToTextureDataAsset;
}

TArray<FName> URecallVertexAnimAsset::GetAllAnimationNames() const
{
	TArray<FName> Results;

#if WITH_EDITOR
	if (AnimToTextureDataAsset)
	{
		Results.Reserve(AnimToTextureDataAsset->AnimSequences.Num());

		for (int32 AnimIndex = 0; AnimIndex < AnimToTextureDataAsset->AnimSequences.Num(); AnimIndex++)
		{
			const FAnimToTextureAnimSequenceInfo& AnimSequenceInfo = AnimToTextureDataAsset->AnimSequences[AnimIndex];
			if (AnimSequenceInfo.AnimSequence)
			{
				Results.Add(AnimSequenceInfo.AnimSequence->GetFName());
			}
		}
	}
#endif // WITH_EDITOR

	return Results;
}
