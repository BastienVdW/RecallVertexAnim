// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "RecallVertexAnimActorFactory.h"

#include "Actor/RecallVertexAnimActor.h"
#include "Representation/Actor/RecallVertexAnimActorTypes.h"
#include "Representation/Actor/Pool/RecallVertexAnimActorPool.h"
#include "StructUtils/InstancedStruct.h"

//----------------------------------------------------------------------//
// URecallVertexAnimActorFactory
//----------------------------------------------------------------------//
TSharedPtr<IRecallObjectPoolInterface> URecallVertexAnimActorFactory::BuildActorPool() const
{
	return MakeShared<FRecallVertexAnimActorPool>();
}

FName URecallVertexAnimActorFactory::GetActorPoolName(const FInstancedStruct& Desc) const
{
	const FRecallVertexAnimActorDesc& VertexAnimDesc = Desc.Get<FRecallVertexAnimActorDesc>();
	if (!VertexAnimDesc.Blueprint.IsNull())
	{
		return *VertexAnimDesc.Blueprint.GetAssetName();
	}
	else
	{
		return ARecallVertexAnimActor::StaticClass()->GetFName();
	}
}
