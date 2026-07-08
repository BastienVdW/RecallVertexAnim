// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "Representation/Actor/RecallActorRepresentationTypes.h"

#include "RecallVertexAnimActorTypes.generated.h"

USTRUCT(BlueprintType)
struct RECALLVERTEXANIMMODULE_API FRecallVertexAnimActorDesc : public FRecallActorRepresentationDesc
{
	GENERATED_BODY()

	FRecallVertexAnimActorDesc();

	/** Blueprint used for the representation of our entity. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowedClasses = "/Script/RecallVertexAnimModule.RecallVertexAnimActor"))
	FSoftClassPath Blueprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMesh> StaticMesh;

	virtual FSoftClassPath GetSoftClassPath() const override { return Blueprint; }
};
