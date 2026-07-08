// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "RecallVertexAnimDebugMenuSubsystem.h"

#include "Debug/DebugMenuInterface.h"
#include "System/Debug/DebugMenuSubsystem.h"

void URecallVertexAnimDebugMenuSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency<UDebugMenuSubsystem>();
	
#if WITH_DEBUG_MENU
	DebugMenuSubsystem = UGameInstance::GetSubsystem<UDebugMenuSubsystem>(GetGameInstance());
	if (DebugMenuSubsystem.IsValid())
	{
		CreateDebugMenuItems(DebugMenuSubsystem->GetMutableDebugMenu());
	}
#endif // WITH_DEBUG_MENU
}

void URecallVertexAnimDebugMenuSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
#if WITH_DEBUG_MENU
	DebugMenuSubsystem.Reset();
#endif // WITH_DEBUG_MENU
}

void URecallVertexAnimDebugMenuSubsystem::Tick(float DeltaTime)
{
}

TStatId URecallVertexAnimDebugMenuSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(URecallGameplayDebugMenuContent, STATGROUP_Tickables);
}

void URecallVertexAnimDebugMenuSubsystem::CreateDebugMenuItems(IDebugMenu& DebugMenu)
{
#if WITH_DEBUG_MENU
	// Vertex Anim
	{
		DebugMenu.AddItem_Bool(TEXT("VA"), "Show Vertex Anim Info", false, TEXT("recall.VA.ShowInfo"));
	}
#endif // WITH_DEBUG_MENU
}
