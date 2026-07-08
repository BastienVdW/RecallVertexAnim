// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "Engine/DataAsset.h"

#include "RecallAnimToTexturePreset.generated.h"

class UAnimToTextureDataAsset;

USTRUCT(BlueprintType)
struct FRecallAnimToTexturePreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> SourceSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Bone Data Asset")
	TObjectPtr<UAnimToTextureDataAsset> AnimToTextureDataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> BonePositionTexture;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> BoneRotationTexture;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> BoneWeightTexture;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UAnimSequence>> Animations;
};

UCLASS()
class RECALLVERTEXANIMMODULE_API URecallAnimToTexturePresetAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Preset)
	FRecallAnimToTexturePreset Preset;
};
