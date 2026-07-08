// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "MassEntityTraitBase.h"
#include "Representation/Actor/RecallVertexAnimActorTypes.h"

#include "RecallVertexAnimActorTraits.generated.h"

UCLASS(meta=(DisplayName="MS Vertex Anim Actor"))
class URecallVertexAnimActorTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

public:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ShowOnlyInnerProperties))
	FRecallVertexAnimActorDesc Definition;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseSharedInstance = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform Offset = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="!bUseSharedInstance"))
	bool bUseActorScale = true;
};
