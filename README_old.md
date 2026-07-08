# RecallVertexAnim - Unreal Engine Plugin

This is the **RecallVertexAnim** plugin, a high-performance vertex animation system built on top of the Recall ECS framework, providing vertex animation representation for deterministic multiplayer games with large numbers of animated entities.

## About RecallVertexAnim

RecallVertexAnim is a specialized vertex animation system that extends the Recall ECS framework with complete AnimToTexture integration. It provides high-performance 3D character animation using vertex animation techniques, enabling thousands of animated characters with minimal performance overhead. The plugin is designed for scenarios where traditional skeletal animation would be too expensive for large entity counts.

The plugin offers:

- **Vertex Animation Integration**: Complete integration with Unreal Engine's AnimToTexture plugin
- **High-Performance Animation**: Vertex animation techniques for massive entity counts
- **ECS Optimization**: Native ECS integration for optimal performance with Recall
- **Instanced Rendering**: Advanced instanced rendering support for vertex animated meshes
- **Animation Asset System**: Comprehensive asset system for managing vertex animations
- **Editor Tools**: Complete editor integration with utility widgets and workflow tools
- **Ability System Integration**: Frame-perfect synchronization with Recall ability system

## Plugin Description

*Vertex Animation for Recall*

This plugin is a **vertex animation framework** built on top of Recall ECS framework with the following key features:
- **AnimToTexture Integration** for converting skeletal animations to vertex animations
- **Instanced Rendering** for massive numbers of animated characters
- **ECS Processing** optimized for vertex animation updates and management
- **Asset Management** for vertex animation data and configuration
- **Editor Tools** for streamlined vertex animation workflow
- **Performance Optimization** designed for thousands of animated entities simultaneously

## Origin and Development

**Based on**: Recall ECS Framework + AnimToTexture Plugin
**Enhanced by**: Bastien Van de Walle
**Purpose**: High-performance vertex animation system for Recall ECS
**Architecture**: Vertex animation framework with instanced rendering optimization
**Inspiration**: Games requiring massive numbers of animated characters with optimal performance

## Core Modules

### RecallVertexAnimModule (Runtime)
**Complete vertex animation system and processing**

#### Animation Asset System
- **Vertex Animation Assets**: `URecallVertexAnimAsset` - Data assets for vertex animation configuration
- **AnimToTexture Integration**: Complete integration with UE5's AnimToTexture plugin
- **Animation Presets**: `URecallAnimToTexturePreset` - Preset configurations for common animation workflows
- **Asset Management**: Efficient loading and management of vertex animation data

#### Actor System
- **Vertex Animation Actors**: `ARecallVertexAnimActor` - Specialized actors for vertex animated characters
- **Actor Types**: `FRecallVertexAnimActorDesc` - Actor descriptors for entity representation
- **Actor Factory**: `URecallVertexAnimActorFactory` - Efficient actor creation system
- **Actor Pool**: `URecallVertexAnimActorPool` - Performance-optimized actor pooling

#### ECS Framework
- **Fragment System**: ECS fragments for vertex animation configuration and state
- **Instance Fragments**: `FRecallVertexAnimInstanceFragment` - Per-entity animation data
- **Shared Fragments**: `FRecallVertexAnimConstSharedFragment` - Shared animation configuration
- **Processor System**: ECS processors for vertex animation updates and management

#### Instanced Rendering
- **Instance Management**: Advanced instanced rendering for vertex animated meshes
- **Instance Fragments**: `FRecallVertexAnimInstanceSharedFragment` - Shared instance data
- **Performance Optimization**: Optimized rendering for thousands of animated entities
- **Transform Management**: Efficient transform updates for instanced rendering

#### Ability Integration
- **Animation Types**: `RecallVertexAnimAbilityAnimationTypes` - Integration with ability system
- **Frame Synchronization**: Frame-perfect synchronization with ability animations
- **Animation Events**: Event system for ability-driven animation changes
- **State Management**: Animation state coordination with ability system

### RecallVertexAnimEditor (Editor)
**Editor tools and workflow utilities**

#### Editor Widgets
- **Utility Widget**: `URecallVertexAnimEditorUtilityWidget` - Comprehensive editor tools
- **Asset Workflow**: Streamlined workflow for creating vertex animation assets
- **Preview Tools**: Real-time preview of vertex animations in editor
- **Batch Processing**: Batch processing tools for animation asset creation

#### Debug System
- **Debug Subsystem**: `URecallVertexAnimDebugMenuSubsystem` - Debug tools and visualization
- **Performance Monitoring**: Real-time performance monitoring for vertex animation
- **Visual Debugging**: Visual debugging tools for animation states and instances
- **Profiling Integration**: Integration with Unreal Engine's profiling tools

## Key Features

### High-Performance Vertex Animation
- **Dual Module Design**: Runtime and editor modules for complete vertex animation workflow
- **ECS Integration**: Native integration with Recall ECS framework
- **Deterministic Animation**: All animation systems designed for deterministic multiplayer
- **Massive Scale**: Designed for thousands of animated entities simultaneously
- **Performance Focus**: Optimized for scenarios where skeletal animation is too expensive

### Advanced AnimToTexture Integration
- **Asset System**: Comprehensive asset system for AnimToTexture data management
- **Animation Selection**: Dynamic animation selection and playback
- **Preset System**: Preset configurations for common animation workflows
- **Optimization**: Optimized asset loading and memory management

### Instanced Rendering System
- **Instance Management**: Advanced instance management for vertex animated meshes
- **Rendering Optimization**: Optimized rendering pipeline for massive entity counts
- **Transform Updates**: Efficient transform updates for instanced characters
- **Culling Integration**: Integration with Unreal Engine's culling systems

### Editor Integration
- **Workflow Tools**: Complete editor workflow for vertex animation creation
- **Utility Widgets**: Editor utility widgets for asset management
- **Preview System**: Real-time preview and testing of vertex animations
- **Debug Tools**: Comprehensive debugging and profiling tools

## Architecture Overview

### Vertex Animation System Architecture
```
RecallVertexAnim Framework
├── Runtime Module (Assets, actors, ECS, instancing, ability integration)
└── Editor Module (Workflow tools, utility widgets, debug system)
```

### Animation Workflow
```
Skeletal Animation → AnimToTexture → Vertex Animation Asset → ECS Entity → Instanced Rendering
```

### Instance Management Flow
```
Entity Creation → Instance Allocation → Transform Updates → Rendering Optimization
```

### Integration Flow
```
Recall ECS → RecallVertexAnim → AnimToTexture → Rendering Pipeline
```

## Integration

The plugin integrates with:
- **Recall Framework**: Core ECS framework with rollback support
- **RecallGameplay**: Gameplay systems integration for vertex animation
- **RecallAbility**: Ability system for synchronized vertex animations
- **AnimToTexture**: Unreal Engine's vertex animation plugin
- **VariableCollection**: Dynamic configuration and parameter management
- **EditorScriptingUtilities**: Editor scripting for workflow automation

## Key Differences from Standard Animation Systems

### Performance Architecture
- **Standard Systems**: Skeletal animation with individual character processing
- **RecallVertexAnim**: Vertex animation with instanced rendering for massive scale

### ECS Integration
- **Standard Systems**: Component-based animation with individual character management
- **RecallVertexAnim**: Native ECS entity-based animation processing

### Scalability Design
- **Standard Systems**: Limited scalability due to skeletal animation overhead
- **RecallVertexAnim**: Designed for thousands of animated entities simultaneously

### Memory Efficiency
- **Standard Systems**: High memory overhead per animated character
- **RecallVertexAnim**: Shared vertex animation data with minimal per-entity overhead

## Configuration

### Basic Setup
1. **Enable Plugin**: Enable RecallVertexAnim in project plugins (disabled by default)
2. **Configure Dependencies**: Ensure Recall, AnimToTexture, and other dependencies are enabled
3. **Setup Assets**: Configure vertex animation assets using AnimToTexture workflow
4. **Initialize Systems**: Set up vertex animation actors and instanced rendering

### Vertex Animation Configuration
1. **Asset Creation**: Use AnimToTexture to convert skeletal animations to vertex animations
2. **Animation Assets**: Configure RecallVertexAnimAsset for animation selection
3. **Actor Setup**: Configure vertex animation actors with appropriate meshes
4. **Instance Configuration**: Set up instanced rendering for performance optimization

## Best Practices

### Vertex Animation Design
- **Deterministic Logic**: Ensure all animation logic is deterministic for multiplayer
- **Performance Focus**: Design for massive scale with thousands of entities
- **Memory Efficiency**: Use shared animation data and minimal per-entity state
- **Asset Optimization**: Optimize vertex animation assets for memory and performance

### Instancing Design
- **Instance Management**: Use instancing efficiently for similar animated entities
- **Transform Optimization**: Minimize transform updates and state changes
- **Culling Integration**: Leverage culling systems for performance optimization
- **Memory Patterns**: Design memory access patterns for optimal cache performance

### Animation Workflow
- **Asset Pipeline**: Establish efficient pipeline from skeletal to vertex animation
- **Quality Balance**: Balance animation quality with performance requirements
- **LOD Integration**: Use level-of-detail systems for distance-based optimization
- **Streaming**: Consider streaming strategies for large animation datasets

## Technical Features

### Animation System
- **Vertex Animation**: High-performance vertex animation playback and management
- **Asset Management**: Efficient loading and management of animation assets
- **Frame Synchronization**: Frame-perfect synchronization with ability system
- **Performance**: Optimized animation processing for ECS architecture

### Instance System
- **Instanced Rendering**: Advanced instanced rendering for vertex animated meshes
- **Instance Allocation**: Efficient instance allocation and management
- **Transform Updates**: Optimized transform updates for large instance counts
- **Culling Integration**: Integration with rendering culling systems

### Editor Tools
- **Workflow Integration**: Complete workflow integration with Unreal Engine editor
- **Asset Creation**: Tools for efficient vertex animation asset creation
- **Debug Visualization**: Visual debugging tools for animation states
- **Performance Monitoring**: Real-time performance monitoring and profiling

## Module Dependencies

### Required Plugins
- **Recall**: Core ECS framework (Required)
- **RecallGameplay**: Gameplay systems integration (Required)
- **RecallAbility**: Ability system integration (Required)
- **AnimToTexture**: Vertex animation conversion plugin (Required)
- **VariableCollection**: Dynamic configuration management (Required)
- **EditorScriptingUtilities**: Editor workflow automation (Required)

### Framework Integration
- **Actor Factory System**: Integration with Recall actor representation
- **Fragment System**: Custom fragments for vertex animation configuration
- **Processor System**: ECS processors for vertex animation updates

## Performance Characteristics

### Animation Performance
- **Entity Processing**: Optimized for thousands of vertex animated entities
- **Animation Updates**: Minimal overhead for vertex animation state updates
- **Memory Usage**: Shared animation data with minimal per-entity memory
- **Frame Synchronization**: Efficient synchronization with ability system

### Rendering Performance
- **Instanced Rendering**: High-performance instanced rendering for similar entities
- **Culling Integration**: Efficient culling for large numbers of animated entities
- **Transform Updates**: Optimized transform updates for instanced rendering
- **GPU Optimization**: GPU-optimized vertex animation processing

### System Integration
- **ECS Performance**: Leverages ECS performance for vertex animation processing
- **Memory Efficiency**: Optimized memory patterns for animation data
- **Asset Loading**: Efficient asset loading and streaming for large datasets
- **Performance Scaling**: Linear performance scaling with entity complexity

## Content Integration

### Animation Assets
- **Vertex Animation Assets**: Pre-configured vertex animation assets for common characters
- **Animation Presets**: Template presets for different animation workflows
- **Material Integration**: Materials and shaders for vertex animation rendering
- **Performance Assets**: Optimized assets for high-performance scenarios

### Editor Assets
- **Utility Widgets**: Editor utility widgets for asset creation and management
- **Workflow Templates**: Template workflows for vertex animation creation
- **Debug Assets**: Debug visualization assets and tools
- **Performance Testing**: Performance testing assets and benchmarks

## Developer Notes

- **Author**: Bastien Van de Walle
- **Category**: Gameplay
- **Version**: 1.0
- **Default State**: Disabled by default (opt-in for high-performance animation projects)
- **Content Support**: Enabled for vertex animation assets and editor tools
- **Framework**: Built on Recall ECS with AnimToTexture integration
- **Performance Focus**: Designed for massive scale animated entity scenarios

## Extension Points

The plugin provides extension points for:
- **Custom Animation Assets**: Implement specialized vertex animation asset types
- **Custom Actors**: Create custom vertex animation actors with specialized functionality
- **Custom Processors**: Create custom ECS processors for specialized animation needs
- **Custom Materials**: Implement specialized materials for vertex animation rendering
- **Custom Editor Tools**: Extend editor workflow with custom tools and widgets

## Advanced Features

### Performance Optimization
- **LOD System**: Level-of-detail system for vertex animations based on distance
- **Streaming**: Animation asset streaming for large datasets
- **Compression**: Animation data compression for memory optimization
- **Batch Processing**: Batch processing for animation updates and state changes

### Rendering Features
- **Advanced Instancing**: Advanced instanced rendering with per-instance variation
- **Culling Optimization**: Advanced culling strategies for vertex animated entities
- **GPU Processing**: GPU-based vertex animation processing and updates
- **Multi-Threading**: Multi-threaded animation processing for performance

### Editor Features
- **Batch Workflows**: Batch processing workflows for animation asset creation
- **Asset Validation**: Automated validation of vertex animation assets
- **Performance Profiling**: Detailed performance profiling and optimization tools
- **Integration Tools**: Integration tools for external animation software

## Testing Framework

### Automated Testing
- **Unit Tests**: Comprehensive unit tests for all vertex animation systems
- **Integration Tests**: Integration tests between ECS and animation systems
- **Performance Tests**: Performance testing for massive numbers of animated entities
- **Asset Tests**: Automated testing of animation asset creation and loading

### Development Testing
- **Debug Visualization**: Visual debugging for vertex animation states and instances
- **Performance Profiling**: Built-in profiling for vertex animation performance
- **Stress Testing**: Stress testing with thousands of animated entities
- **Asset Validation**: Validation tools for animation asset quality

## Use Cases

### Game Development
- **Strategy Games**: RTS games with hundreds or thousands of animated units
- **MMO Games**: Massive multiplayer games with large numbers of animated characters
- **Mobile Games**: Mobile games requiring high performance with limited resources
- **Crowd Simulation**: Crowd simulation with large numbers of animated characters

### System Integration
- **Existing Projects**: Integration with existing Unreal Engine projects
- **Custom Animation**: Platform for custom high-performance animation systems
- **Performance Critical**: Applications requiring maximum animation performance
- **Large Scale**: Applications with massive numbers of animated entities

## Credits

- **Framework Foundation**: Recall ECS Framework + AnimToTexture Plugin
- **Implementation**: Bastien Van de Walle - High-performance vertex animation system
- **ECS Integration**: Native integration with Recall deterministic framework
- **Performance Design**: Advanced vertex animation systems for massive scale
- **Editor Integration**: Comprehensive editor workflow and tooling