# RecallVertexAnim

Vertex animation (AnimToTexture) integration for the Recall ECS framework — high-performance, instanced 3D character animation for deterministic multiplayer with large entity counts.

## Overview

RecallVertexAnim bakes skeletal animation to vertex-animation textures via Unreal's AnimToTexture plugin, then drives thousands of ECS entities from that data using instanced rendering, avoiding per-entity skeletal animation cost.

## Modules

### RecallVertexAnimModule (Runtime)

- **`URecallVertexAnimAsset`** — data asset wrapping AnimToTexture animation data and selection presets
- **`ARecallVertexAnimActor`** / **`FRecallVertexAnimActorDesc`** — actor and descriptor for vertex-animated entity representation
- **`URecallVertexAnimActorFactory`** / **`URecallVertexAnimActorPool`** — actor creation and pooling
- **`FRecallVertexAnimInstanceFragment`** / **`FRecallVertexAnimConstSharedFragment`** / **`FRecallVertexAnimInstanceSharedFragment`** — per-entity and shared ECS fragments for animation state and instanced rendering
- Ability-system animation type integration for frame-synced animation events

### RecallVertexAnimEditor (Editor)

- **`URecallVertexAnimEditorUtilityWidget`** — editor workflow tools for creating/previewing vertex animation assets
- **`URecallVertexAnimDebugMenuSubsystem`** — debug/visualization tools for animation state and instancing

## Dependencies

- Recall
- RecallGameplay
- RecallAbility
- AnimToTexture
- VariableCollection
- EditorScriptingUtilities

## Setup

1. Enable the plugin (disabled by default).
2. Use AnimToTexture to bake skeletal animations to vertex animation data.
3. Create a `URecallVertexAnimAsset` referencing the baked data and configure `ARecallVertexAnimActor` meshes.

## Notes

- Author: Bastien Van de Walle
- Category: Gameplay
- Disabled by default; intended for scenarios with large numbers of animated entities where skeletal animation is too costly