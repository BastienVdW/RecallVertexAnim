// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "Engine/DataAsset.h"

#include "RecallVertexAnimAsset.generated.h"

class UAnimToTextureDataAsset;

UCLASS()
class RECALLVERTEXANIMMODULE_API URecallVertexAnimAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	int32 GetAnimationIndex() const;
	const UAnimToTextureDataAsset* GetAnimToTextureDataAsset() const;
	
protected:
	/**
	 * For now, we reference the whole asset, but there is a lot of data that we might not need.
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimToTextureDataAsset> AnimToTextureDataAsset;

	/**
	 * Name of the animation to play.
	 */
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetAllAnimationNames"))
	FName AnimationName = NAME_None;

	UFUNCTION()
	TArray<FName> GetAllAnimationNames() const;
};
