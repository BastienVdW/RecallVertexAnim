// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "MassEntityTypes.h"
#include "Representation/Actor/RecallVertexAnimActorTypes.h"
#include "System/Actor/RecallActorTypes.h"

#include "RecallVertexAnimFragments.generated.h"

USTRUCT()
struct FRecallVertexAnimInstanceFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FTransform Offset = FTransform::Identity;
};

USTRUCT()
struct FRecallVertexAnimInstanceSharedFragment : public FMassSharedFragment
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FName InstanceName = NAME_None;
	
	UPROPERTY(VisibleAnywhere)
	FRecallActorHandle InstanceHandle;
	
	UPROPERTY(VisibleAnywhere)
	int32 InstanceCount = 0;
};

USTRUCT()
struct FRecallVertexAnimConstSharedFragment : public FMassConstSharedFragment
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FRecallVertexAnimActorDesc Definition;
};
