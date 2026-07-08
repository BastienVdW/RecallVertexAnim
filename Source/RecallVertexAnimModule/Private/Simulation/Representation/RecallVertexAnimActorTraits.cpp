// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "Simulation/Representation/RecallVertexAnimActorTraits.h"

#include "MassEntityTemplateRegistry.h"
#include "MassEntityView.h"
#include "Simulation/Representation/RecallActorRepresentationFragments.h"
#include "Simulation/Representation/RecallVertexAnimFragments.h"
#include "Simulation/Transform/RecallTransformFragments.h"
#include "Utility/Trait/RecallTraitUtils.h"

//----------------------------------------------------------------------//
// URecallVertexAnimActorTrait
//----------------------------------------------------------------------//
void URecallVertexAnimActorTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	FMassEntityManager& EntityManager = UE::Mass::Utils::GetEntityManagerChecked(World);
	
	BuildContext.RequireFragment<FRecallTransformFragment>();

	if (bUseSharedInstance)
	{
		FRecallVertexAnimInstanceFragment& InstanceFragment = BuildContext.AddFragment_GetRef<FRecallVertexAnimInstanceFragment>();
		InstanceFragment.Offset = Offset;
		
		FRecallVertexAnimInstanceSharedFragment InstanceSharedFragment;
		InstanceSharedFragment.InstanceName = Definition.StaticMesh ? Definition.StaticMesh->GetFName() : NAME_None;
		
		BuildContext.AddSharedFragment(EntityManager.GetOrCreateSharedFragment(InstanceSharedFragment));
	}
	else
	{		
		FRecallActorRepresentationFragment& ActorFragment = BuildContext.AddFragment_GetRef<FRecallActorRepresentationFragment>();
		ActorFragment.Offset = Offset;

		if (bUseActorScale)
		{
			BuildContext.GetMutableObjectFragmentInitializers().Add([this, &World](UObject& Owner, FMassEntityView& EntityView, const EMassTranslationDirection CurrentDirection)
				{
					if (const AActor* Actor = Recall::Trait::Utils::AsActor(Owner))
					{
						FRecallActorRepresentationFragment& ActorRepresentationFragment = EntityView.GetFragmentData<FRecallActorRepresentationFragment>();
						ActorRepresentationFragment.Scale *= Actor->GetActorScale3D();
					}
				}
			);
		}
	}

	FRecallVertexAnimConstSharedFragment SharedFragment;
	SharedFragment.Definition = Definition;

	BuildContext.AddConstSharedFragment(EntityManager.GetOrCreateConstSharedFragment(SharedFragment));
}
