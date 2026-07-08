// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "CoreMinimal.h"
#include "Representation/Actor/Pool/RecallObjectPoolTypes.h"

class RECALLVERTEXANIMMODULE_API FRecallVertexAnimActorPool : public FRecallObjectPool
{
protected:
	virtual AActor* CreateObject(const FInstancedStruct& Desc) override final;
	virtual void InitObject(AActor* Actor, const FInstancedStruct& Desc) override final;

	virtual void EnableObject(AActor* Actor) override final;
	virtual void DisableObject(AActor* Actor) override final;
};
