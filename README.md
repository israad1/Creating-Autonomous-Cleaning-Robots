```
  ██████╗██╗     ███████╗ █████╗ ███╗  ██╗██████╗  ██████╗ ████████╗
 ██╔════╝██║     ██╔════╝██╔══██╗████╗ ██║██╔══██╗██╔═══██╗╚══██╔══╝
 ██║     ██║     █████╗  ███████║██╔██╗██║██████╔╝██║   ██║   ██║
 ██║     ██║     ██╔══╝  ██╔══██║██║╚████║██╔══██╗██║   ██║   ██║
 ╚██████╗███████╗███████╗██║  ██║██║ ╚███║██████╔╝╚██████╔╝   ██║
  ╚═════╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚══╝╚═════╝  ╚═════╝   ╚═╝
          Autonomous Robot Fleet Manager  ─  v2.0
```

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-20-blue?style=for-the-badge&logo=cplusplus" alt="C++20">
  <img src="https://img.shields.io/badge/Build-CMake_3.16+-green?style=for-the-badge&logo=cmake" alt="CMake">
  <img src="https://img.shields.io/badge/Tests-Catch2_v3-orange?style=for-the-badge" alt="Catch2">
  <img src="https://img.shields.io/badge/License-MIT-lightgrey?style=for-the-badge" alt="MIT">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=for-the-badge" alt="Status">
</p>

---

## What is this?

**CleanBot** is a fleet-management simulator for a team of autonomous cleaning robots working across a multi-room building.
You command the fleet from a rich terminal UI: assign robots to dirty rooms, watch them clean in simulated time, respond to failures, and use the built-in smart auto-assign to let the system optimise itself.

Originally built as a CMSC 322 Software Engineering Practicum project (Fall 2023), v2.0 adds a fully redesigned CLI, four new robot capabilities, smart scheduling, session analytics, and a suite of bug fixes.

---

## Features

| Feature | Description |
|---|---|
| **4 robot types** | Sweeper, Mopper, Scrubber, Vacuumer — each matched to room surfaces |
| **2 robot sizes** | Small (agile) and Large (high-throughput) |
| **Smart auto-assign** | Scores every robot/room pair by battery level, dirtiness, and priority; assigns the best matches automatically |
| **Room priority flags** | Mark critical rooms for preferential scheduling |
| **Live dashboard** | Full-screen fleet + building overview with colour-coded progress bars |
| **Session analytics** | Assignments, completions, failures, and efficiency over time |
| **Technician dispatch** | Queue failed robots for repair |
| **Battery prediction** | Warns before sending a robot it cannot finish the job |
| **Rich terminal UI** | ANSI colours, Unicode progress bars, bold tables — no external TUI library needed |
| **CSV configuration** | Drop-in robot and room definitions via `input.csv` |
| **Comprehensive tests** | Catch2 unit tests for every class |

---

## Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                        main.cpp  (CLI)                          │
│  dashboard · auto-assign · stats · assign · view · time …       │
└──────────┬──────────────────────────────┬───────────────────────┘
           │                              │
    ┌──────▼──────┐                ┌──────▼──────┐
    │   Manager   │                │   Timer     │
    │  (control)  │                │  (sim time) │
    └──┬──────────┘                └─────────────┘
       │ coordinates
  ┌────▼────┐   ┌──────────┐   ┌───────────┐   ┌────────────┐
  │  Fleet  │   │ Building │   │Technician │   │  Robot(s)  │
  │ [Robot] │   │  [Room]  │   │  (queue)  │   │ clean/move │
  └─────────┘   └──────────┘   └───────────┘   └────────────┘
```

### Class summary

| Class | Responsibility |
|---|---|
| `Robot` | State machine: location, battery, job, busy/failed flags; performs `clean()` / `charge()` / `move()` |
| `Room` | Tracks dimensions and per-type cleanliness percentages (swept / mopped / scrubbed / vacuumed) |
| `Fleet` | Container that splits robots into available / busy / all lists |
| `Building` | Container that splits rooms into clean / dirty / all lists |
| `Manager` | Validates and executes assignments; drives display methods |
| `Technician` | FIFO repair queue; fixes one robot per time step |
| `Timer` | Global simulation clock |

---

## Directory layout

```
.
├── app/
│   ├── main.cpp          # Interactive CLI  (v2.0 rewrite)
│   ├── input.csv         # Robot & room definitions
│   └── inputtemplate.csv # Template showing the CSV schema
├── include/libclean/
│   ├── robot.hpp
│   ├── fleet.hpp
│   ├── room.hpp
│   ├── building.hpp
│   ├── manager.hpp
│   ├── technician.hpp
│   ├── timer.hpp
│   └── ui.hpp            # Header-only terminal UI (v2.0)
├── src/
│   ├── robot.cpp
│   ├── fleet.cpp
│   ├── room.cpp
│   ├── building.cpp
│   ├── manager.cpp
│   ├── technician.cpp
│   └── timer.cpp
├── tests/                # Catch2 unit tests
└── docs/                 # Design diagrams and sprint notes
```

---

## Quick start

### Requirements

- CMake ≥ 3.16
- A C++20-capable compiler (GCC 10+, Clang 12+, MSVC 2019+)
- Internet access on first build (CPM fetches `fmt` and `Catch2` automatically)

### Build

```bash
git clone <repo-url>
cd Creating-Autonomous-Cleaning-Robots
cmake -S . -B build
cmake --build build
```

### Run

```bash
# copy input.csv next to the binary (or run from the app/ dir)
cp app/input.csv build/app/
./build/app/main
```

The program searches for `input.csv` in `./`, `../app/`, `../../app/`, and `app/` automatically.

### Run tests

```bash
cmake --build build --target Test
./build/tests/Test
```

---

## input.csv format

```
robots:
<name> <small|large> <sweeper|mopper|scrubber|vacuumer>

rooms:
<name> <length> <width> <sweepable> <moppable> <scrubbable> <vacuumable>
```

Boolean values: `true` / `false`.

**Example:**

```
robots:
Sweep1  small  sweeper
Mop1    large  mopper
Vac1    small  vacuumer

rooms:
Office    10  20   true   false  true   false
Kitchen   8   9.5  true   false  false  true
Restroom  5   8.3  true   true   true   true
```

---

## Command reference

```
  assign <robot> <room>         Assign a robot to clean a room
  auto                          Smart-assign best robots to dirty rooms
  dashboard                     Full fleet + building overview
  stats                         Session analytics

  display dirty rooms           List dirty rooms
  display clean rooms           List clean rooms
  display all rooms             List every room
  display busy robots           List robots currently working
  display available robots      List robots ready to assign
  display all robots            Full fleet list

  view <robot|room>             Inspect a robot or room in detail
  call technician <robot>       Dispatch the technician to repair a robot
  priority <room>               Toggle high-priority flag on a room

  dirty <room>                  Mark a room dirty (for simulation)
  time <N>                      Advance simulation by N time units
  time until <robot>            Wait until a robot finishes or fails
  exit                          Quit (shows final stats)
  help                          Show this list
```

---

## How smart auto-assign works

```
score = (battery_ratio × 0.4  +  dirtiness × 0.6) × priority_multiplier
```

- **battery_ratio** – robot's current battery / 100
- **dirtiness** – (100 − current_clean_pct) / 100 for the relevant cleaning type
- **priority_multiplier** – 1.5× for rooms marked `priority`, 1.0 otherwise
- Robots with less than 50 % of the estimated battery requirement are skipped
- Each robot and each room can appear in at most one assignment per `auto` call
- All candidate pairs are scored, sorted descending, and greedily assigned

---

## Simulation mechanics

| Mechanic | Detail |
|---|---|
| **Cleaning rate** | Small robot: cleans 5 m² per tick · Large: 12 m² per tick |
| **Battery drain** | 1 % per cleaning tick |
| **Charging** | +5 % per tick while at Base |
| **Move cost** | −5 % battery per move |
| **Random failure** | 1 % chance per tick of a robot breaking down mid-task |
| **Repair** | Technician fixes one queued robot per tick |

---

## Contributing

1. Fork and create a branch: `git checkout -b feature/my-feature`
2. Follow the existing code style (`.clang-format` and `.clang-tidy` are provided)
3. Add or update Catch2 tests in `tests/`
4. Open a PR against `main`

---

## License

MIT — see [LICENSE](LICENSE) for details.

---

*CMSC 322 · Fall 2023 Software Engineering Practicum · University of Maryland*
