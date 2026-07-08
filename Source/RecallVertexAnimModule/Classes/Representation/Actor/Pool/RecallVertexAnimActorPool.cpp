// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "RecallVertexAnimActorPool.h"

#include "Actor/RecallVertexAnimActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Representation/Actor/RecallVertexAnimActorTypes.h"
#include "StructUtils/InstancedStruct.h"

//----------------------------------------------------------------------//
// FRecallVertexAnimActorPool
//----------------------------------------------------------------------//
template<typename ActorType>
static ActorType* SpawnPoolActor(const TWeakObjectPtr<UWorld>& World, const TSubclassOf<ActorType>& Class)
{
	if (World.IsValid())
	{
		ActorType* NewActor = World->SpawnActorDeferred<ActorType>(Class, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		NewActor->bOnlyRelevantToOwner = true;
		NewActor->FinishSpawning(FTransform::Identity);
		return NewActor;
	}

	return nullptr;
}

AActor* FRecallVertexAnimActorPool::CreateObject(const FInstancedStruct& Desc)
{
	return SpawnPoolActor<ARecallVertexAnimActor>(GetContainer().GetWorld(), GetActorClass<ARecallVertexAnimActor>());
}

void FRecallVertexAnimActorPool::InitObject(AActor* Actor, const FInstancedStruct& Desc)
{
	const FRecallVertexAnimActorDesc& VertexAnimDesc = Desc.Get<FRecallVertexAnimActorDesc>();
	
	if (ARecallVertexAnimActor* VertexAnimActor = Cast<ARecallVertexAnimActor>(Actor))
	{
		if (UInstancedStaticMeshComponent* InstancedMeshComponent = VertexAnimActor->GetInstancedMeshComponent())
		{
			InstancedMeshComponent->SetStaticMesh(VertexAnimDesc.StaticMesh);
		}
	}
}

void FRecallVertexAnimActorPool::EnableObject(AActor* Actor)
{
	SetActorActive(Actor, true);
}

void FRecallVertexAnimActorPool::DisableObject(AActor* Actor)
{
	SetActorActive(Actor, false);
}
