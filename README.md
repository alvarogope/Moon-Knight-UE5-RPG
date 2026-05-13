# Moon Knight · Dark Fantasy Action RPG

![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine%205-0E1128?style=flat&logo=unrealengine&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus&logoColor=white)
![Blueprints](https://img.shields.io/badge/Blueprints-Visual%20Scripting-0E1128?style=flat&logo=unrealengine&logoColor=white)

A solo-developed dark fantasy action RPG built in Unreal Engine 5, exploring the research question: **"How could quantum computing change video game mechanics in role-playing games?"**

All core systems were designed and implemented from scratch: combat, AI, inventory, equip, dialogue, target lock, and a dual skill tree including a set of quantum-inspired abilities derived from real quantum mechanics principles.

---

## 🎬 Gameplay Preview

[![Moon Knight Gameplay](assets/Poster_MoonKnight.png)](https://youtu.be/MnGfEbrWqQs)

---

## 📸 Screenshots

### Behaviour Tree — Standard Enemy
![Standard Enemy Behaviour Tree](assets/AI_Behaviour_Tree.png)

### Behaviour Tree — Mini-Boss (Werewolf)
![Werewolf Behaviour Tree](assets/Behaviour_Tree_Werewolf.png)

### Combat System — Combo Chain
![Combat System Blueprints](assets/Combat_System_Blueprints.png)

### Sword Trace — Hit Detection
![Sword Trace](assets/Sword_Trace.png)

### Mini-Boss Combat Activation
![MiniBoss Combat](assets/MiniBoss_Combat.png)

### Target Lock System
![Target Lock](assets/Target_Lock.png)

---

## 🔬 Research Premise

The project investigates whether quantum mechanics principles (superposition, entanglement, inversion, orbital force) can be translated into viable, fun gameplay systems for an action RPG — without requiring real quantum hardware.

Each quantum ability in the game is derived from a real quantum concept, reinterpreted as a mechanic that changes how the player interacts with the world. The design framework produced serves as a forward-looking reference for the intersection of quantum computing and game design.

---

## ⚔️ Combat System

Fast-paced, no-block, parry-based encounters built around player aggression and high-risk decision making. The player is rewarded for attacking and punished for passive play.

- **4-hit light attack combo chain** driven by an AttackIndex integer (0-4) routed through a Switch on Int node. After the 4th hit, a combo finisher triggers automatically
- **Combo continuation** managed via a SaveAttack boolean — input during an active attack queues the next in chain
- **Player can be hit mid-combo** — no invincibility frames during attacks
- **Player can dodge mid-combo** at any point in the chain
- **Hit detection** uses a looping Sphere Trace By Channel (radius 120 units, Visibility channel) running every 0.001s between SwordTopPoint and SwordBottomPoint bones. Valid targets are filtered by the Actor Tag "Damageable". On confirmed hit, ApplyDamage is called (base damage 200) and a particle emitter spawns at impact
- **Target Lock** fires a Sphere Trace For Objects (radius 200, range 1500 units) from the camera forward vector, filtering for PhysicsBody and Pawn types, validated by "Damageable" tag. Toggle input clears the lock

---

## 🌳 Dual Skill Tree System

| Tree | Gate | Description |
|---|---|---|
| Knight Abilities | Boss kills | Combat and traversal abilities unlocked by defeating bosses |
| The Power of the Gods | Exploration | Quantum-inspired abilities unlocked by discovering locations |

The two trees are designed to reward different play styles — aggression versus curiosity.

---

## ⚛️ Quantum Ability System (The Power of the Gods)

Each ability is derived from a real quantum mechanics principle and translated into a gameplay mechanic:

| Ability | Quantum Concept | Gameplay Effect |
|---|---|---|
| **Master of Matters** | Matter-wave duality | Control over physical objects in the environment |
| **Instability** | Quantum decoherence | Creates unpredictable, randomised enemy behaviour |
| **Inversion** | NOT gate / time reversal | Reverses an action or effect that has already occurred |
| **Elliptical Force** | Orbital mechanics | Applies orbital force to enemies or objects |
| **Double Superposition** | Quantum superposition | Player exists in two states simultaneously |

---

## 🤖 Enemy AI

Two distinct enemy types, each driven by a custom Behaviour Tree:

### Standard Enemy (BD_AI)

```
Patrol → Investigate (on sound) → Chase → Attack
```

- Patrols to random points within 3000 unit radius, waits 2 seconds at each point
- Investigates last known player position on hearing detection, waits 9 seconds before resuming patrol
- Chases player (BT_Task_ChaseTarget), stops at 100 units
- Attacks when within 250 units (BT_Task_AttackTarget)
- BT_Service_GetDistanceToTarget ticks every 0.40-0.60 seconds to update inMeleeAttackRange

### Mini-Boss / Werewolf (BD_Werewolf)

```
Patrol / Combat (Chase + Attack simultaneously via Simple Parallel)
```

- No investigation state: player detection triggers immediate combat
- Simple Parallel node chases and attacks at the same time
- Max health: 350, tracked via dedicated WB_Werewolf_H widget
- Wider aggro radius than standard enemy
- Single powerful attack replaces the standard attack set

---

## 🗺️ World Areas

| Area | Description |
|---|---|
| Tutorial — Centralis | Introduces combat, parry system, and core movement |
| The Woods | First open area with standard enemies and environmental storytelling |
| Misty Lands | Mid-game area introducing ranged enemy types |
| Frozen Mountains | Final area with boss encounters and full quantum ability access |

---

## 🎒 Inventory and Equip System

- Slot-based inventory backed by a Data Table using S_Slots and S_Items structs
- Item rows store: Name, Damage, Icon, StaticMesh, Type, WeaponSocket
- Equipping calls Get Data Table Row, breaks S_Items, and swaps the StaticMesh on the weapon socket
- Supports Sword and Bow equip slots
- **Critical bug fix:** equipment state persistence across level loads was resolved by rethinking state ownership at level load using Game Instance rather than per-actor variables

---

## 🖥️ UI Design

| Element | Description |
|---|---|
| Player health bar | Displayed as moon phases integrated into the player's armour |
| Quantum cooldown | Circular indicator showing quantum ability charge |
| Enemy health bar | Shared across standard enemies and player via character type selector |
| Mini-Boss health bar | Dedicated widget (WB_Werewolf_H), activated on aggro |
| Inventory screen | Slot-based UMG widget |
| Dialogue box | Linear NPC conversations with speaker name display |

---

## 🏗️ Repository Structure

```
moon-knight-ue5-rpg/
├── Source/
│   ├── MoonKnightRPG.h              # C++ systems architecture — structs, enums, constants
│   ├── APlayerCharacter.h/.cpp      # Player character base class
│   ├── UQuantumAbilityComponent.h/.cpp  # Quantum ability system component
│   ├── AEnemyAIController.h/.cpp    # Enemy AI controller base
│   └── UMoonKnightGameInstance.h/.cpp   # Game instance — equip state persistence
├── assets/
│   ├── AI_Behaviour_Tree.png
│   ├── Behaviour_Tree_Werewolf.png
│   ├── Combat_System_Blueprints.png
│   ├── Sword_Trace.png
│   ├── MiniBoss_Combat.png
│   └── Target_Lock.png
└── README.md
```

> The full UE5 project (Blueprints, maps, assets) is not hosted on GitHub due to binary file size constraints. This repository documents the systems architecture, C++ foundations, and all tuning values extracted directly from the Blueprint implementation.

**Blueprint vs C++ split:**

| System | Implementation |
|---|---|
| Combat combo chain, sword trace, ApplyDamage | Blueprint (BPC_Attack System) |
| Enemy Behaviour Trees, Blackboard, patrol tasks | Blueprint (BD_AI, BD_Werewolf) |
| AI perception, detection, sight stimulus | Blueprint (AI Detection BP) |
| Inventory Data Table, equip socket swap | Blueprint (BPC_Equipment System) |
| Player base class, component registration | C++ (APlayerCharacter) |
| Enemy AI controller base | C++ (AEnemyAIController) |
| Game Instance equip persistence | C++ (UMoonKnightGameInstance) |

---

## 🛠️ Tech Stack

| | |
|---|---|
| Engine | Unreal Engine 5 |
| Scripting | Blueprint Visual Scripting |
| Code | C++ (base classes, components, game instance) |
| UI | UMG (Unreal Motion Graphics) |
| AI | Behaviour Trees, UE5 Perception System |
| Input | Enhanced Input System |
| Animation | Animation Montages, Animation Notify system |

---

## 🚀 Running Locally

This project requires Unreal Engine 5 installed via the Epic Games Launcher.

```bash
# Clone the repository
git clone https://github.com/alvarogope/Moon-Knight-UE5-RPG.git

# Open the .uproject file in Unreal Engine 5
# Right-click the .uproject → Generate Visual Studio project files
# Build and run from the UE5 editor
```

> Note: large asset files (meshes, textures, animations) are not included in this repository. The Source folder and README document the systems architecture independently.

---

## 🔗 Portfolio

Full project breakdown, GDD, and design documentation available at:
[alvarogomezgamedesign.wordpress.com/moon-knight](https://alvarogomezgamedesign.wordpress.com/moon-knight/)
