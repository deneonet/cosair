# Cosair Engine

A 2D Game Engine written in C++, following ["The Cherno's Game Engine Series"](https://thecherno.com/engine) with own modifications, like:

- Materials
- Transforms
- API Changes
- Texture Library
- Bindless Textures
- Performance Optimizations
- Better error handling (texture fallbacks, fragment shader fallbacks)

## Try it
- Clone this repo
- Run `win-generate.bat` inside `scripts`
- Go back & Open the Solution file
- Hit Run
- Build fails first time, just run it again (i'm going to fix this)
- Play Flappybird

[Sandbox & Flappybird](Sandbox/src/README.md)

## Plans

#### Little bit excessive, but plans, in which direction I want to go with this game engine

- Rendering:
    * Lightning
    * Animations
    * Post Processing
    * HDR Rendering
- User Interface:
    * Text Rendering (SDF)
    * Layout System
    * HUD
    * Screens
    * Animations
- Audio:
    * Playback
    * Scripting/Events
    * Effects
    * Mixing
    * Node Editor
- Serialization:
    * Scores/Games, etc.
- Asset System:
    * Memory Mapping
    * Editor Hot Reloading
- Platform Support:
    * x64 Windows, x86 Windows, MacOS, Linux
    * Graphic Drivers (AMD, NVIDIA, Intel)
- Input System:
    * Gamepads/Controllers
    * Actions
- Networking:
    * File downloading
    * HTTP requests
    * Multiplayer
- Optimization:
    * Multi-threaded
    * GPU Offloading
- Localization
- Graphic APIs:
    * Vulkan, DirectX, OpenGL
- Cinematics:
    * Cutscenes
    * Editor
- Editor:
    * Tools
    * Scenes
    * Asset Management
- Scripting:
    * C# Support
    * Lua Support
- Physics:
    * Box2D
    * Scripting/Callbacks
- Particle System:
    * VFX Graph
    * Texture
    * Collision
    * GPU Compute