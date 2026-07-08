// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"

#include "RecallVertexAnimActor.generated.h"

#define MS_VA_INSTANCE_MAX 8192

class URecallAbilityAsset;
class URecallVertexAnimAsset;
struct FAnimToTextureFrameData;

struct FRecallVertexAnimInstance
{
	FTransform Transform = FTransform::Identity;
	const URecallAbilityAsset* Ability = nullptr;
	float Frame = 0.0f;
};

/**
 * Actor that can be used to display vertex animations.
 */
UCLASS(Blueprintable)
class RECALLVERTEXANIMMODULE_API ARecallVertexAnimActor : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	void BatchSetInstances(const TArray<FRecallVertexAnimInstance, TFixedAllocator<MS_VA_INSTANCE_MAX>>& Instances);
	
	void SetInstanceCount(int32 Count);
	void SynchronizeToAbilityFrame(const URecallAbilityAsset* Ability, float Frame, int32 InstanceIndex = 0) const;

	UInstancedStaticMeshComponent* GetInstancedMeshComponent() const { return InstancedMeshComponent; }

	//~ Begin AActor Interface
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	//~ End AActor Interface
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vertex Anim", meta=(ClampMin=1))
	int32 DefaultInstanceCount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vertex Anim")
	bool bAutoPlayData = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vertex Anim", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> InstancedMeshComponent;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Vertex Anim")
	int32 InstanceCount = 0;

	// TArray<FMatrix> BatchTransforms;
	TArray<FAnimToTextureFrameData> BatchData;
	TArray<FInstancedStaticMeshInstanceData> BatchInstanceData;
	TArray<float> BatchCustomData;

	bool bUpdatingInstancedMesh = false;
	
	static bool FindFrameData(const URecallAbilityAsset* Ability, float Frame, FAnimToTextureFrameData& OutFrameData);
};
