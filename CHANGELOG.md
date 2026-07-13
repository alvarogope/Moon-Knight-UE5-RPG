# Moon-Knight UE5 RPG — Change Summary

**Date:** July 2026
**Scope:** Restructure of the C++ codebase from stub headers into a complete, compilable Unreal Engine 5 game module, plus README overhaul.

## Repository structure

- All C++ files moved into the standard UE5 layout: `Source/MoonKnightRPG/`.
- Added module scaffolding required for the code to build as a real UE5 module:
  - `MoonKnightRPG.Build.cs` (declares dependencies: Core, Engine, EnhancedInput, AIModule, GameplayTasks, NavigationSystem, UMG)
  - `MoonKnightRPG.cpp` (registers the primary game module)
- README now states this is the Source half of a UE5 project, with Content (Blueprints, assets) excluded due to size.

## Header fixes (previous stubs would not compile)

- `MoonKnightRPH.h` renamed to `MoonKnightRPG.h` (typo fix) and made the module header.
- `ECombatState` was defined twice (in the shared header and the player header); it is now defined once in `MoonKnightRPG.h` and included where needed.
- All `.generated.h` includes corrected to match their file names (Unreal Header Tool requirement), e.g. `MKPlayerCharacter.h` now includes `MKPlayerCharacter.generated.h`.
- Classes renamed to match file names per UE convention: `AMKPlayerCharacter`, `AMKEnemyAIController`, `UMKGameInstance`.
- Added a `MoonKnightConstants` namespace as the single source of truth for design tuning values (death fade 5.0s, respawn 4.9s, enemy perception radii).

## New implementations (.cpp files)

- **MKPlayerCharacter** — health and damage through the engine's `TakeDamage` override, parry negation, 4-hit combo state machine with reset timer, healing blocked during combat, death sequence (input disabled, 5s fade event to Blueprint, respawn at 4.9s via the Game Instance).
- **MKEnemyAIController** — AI perception (sight and hearing) configured in C++ from the shared constants; stimuli translated into Blackboard keys `TargetActor` and `InvestigateLocation`; Behaviour Tree launched on possess. Decision-making remains in the Behaviour Tree assets.
- **MKGameInstance** — persistent session state: Willow Tree checkpoint registration, respawn at the last checkpoint, equipped weapon persistence across level loads.

## README

- Repository structure block updated to the new layout.
- Blueprint vs C++ split table updated with the real class names; AI perception row moved from Blueprint to C++.
- New **Coding Choices** section documenting the rationale: the Blueprint/C++ hybrid, centralised tuning constants, engine damage pipeline, perception/decision separation, and state ownership following object lifetime.
- New **Integration** subsection with the five steps to wire the Source-only code into the full project (Game Instance setting, Blueprint reparenting, Behaviour Tree assignment, Blackboard keys).
- Screenshots section restored; gameplay video linked via clickable poster image.

## Integration requirements (manual, in-editor)

1. Set `MKGameInstance` as the Game Instance class (Project Settings > Maps & Modes).
2. Reparent the player and enemy controller Blueprints to the new C++ classes.
3. Assign the Behaviour Tree asset on the enemy controller Blueprint.
4. Blackboard must contain `TargetActor` (Object) and `InvestigateLocation` (Vector).