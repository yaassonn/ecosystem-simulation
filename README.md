# 🌿 Ecosystem Simulation

![C++](https://img.shields.io/badge/C%2B%2B-20-blue?style=for-the-badge&logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-Build%20System-darkred?style=for-the-badge&logo=cmake)
![SFML](https://img.shields.io/badge/SFML-2.5.1-green?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Windows%20x64-0078D6?style=for-the-badge)
![Architecture](https://img.shields.io/badge/Architecture-OOP-orange?style=for-the-badge)
![Visualization](https://img.shields.io/badge/Visualization-Real--Time-success?style=for-the-badge)
![Multithreading](https://img.shields.io/badge/Multithreading-std%3A%3Athread-purple?style=for-the-badge)
![Lines of Code](https://img.shields.io/badge/Code%20Size-1433%20lines-brightgreen?style=for-the-badge)

---

## 📌 About the Project

**Ecosystem Simulation** is a C++ course project that models a multi-species ecosystem on a two-dimensional cellular field.

The simulation includes:

- 🌱 **Plants**
- 🐇 **Herbivores**
- 🐺 **Predators**
- 🪨 **Obstacles**

The world evolves step by step. At each tick, the program processes:

- plant growth
- movement of animals
- search for food
- energy loss
- aging
- reproduction
- removal of dead entities

The project uses **object-oriented design**, **graphical visualization**, **configuration via file**, **statistics export**, and **multithreaded computations**.

---

## ✨ Features

- object-oriented architecture
- 2D grid-based ecosystem model
- toroidal world geometry
- real-time graphical visualization with **SFML**
- different environment modes:
  - `normal`
  - `drought`
  - `overgrowth`
- obstacles on the field
- statistics collection and CSV export
- configurable parameters via `config.ini`
- multithreaded parts using `std::thread`
- synchronization using `std::mutex`
- build system based on **CMake**

---

## 🧩 Entities

The simulation world contains the following entity types:

| Entity | Description |
|--------|-------------|
| **Plant** | Food source for herbivores |
| **Herbivore** | Moves around the field, searches for plants, spends energy, reproduces |
| **Predator** | Hunts herbivores, spends energy, reproduces |
| **Obstacle** | Occupies cells and blocks movement |

---

## 🏗 Project Structure

~~~text
ecosystem_sim/
├── include/
│   ├── Animal.hpp
│   ├── Cell.hpp
│   ├── Config.hpp
│   ├── ConsoleRenderer.hpp
│   ├── Entity.hpp
│   ├── Grid.hpp
│   ├── Herbivore.hpp
│   ├── Obstacle.hpp
│   ├── Plant.hpp
│   ├── Predator.hpp
│   ├── Renderer.hpp
│   ├── SimulationEngine.hpp
│   ├── StatisticsCollector.hpp
│   └── Types.hpp
├── src/
│   ├── Animal.cpp
│   ├── Cell.cpp
│   ├── Config.cpp
│   ├── ConsoleRenderer.cpp
│   ├── Entity.cpp
│   ├── Grid.cpp
│   ├── Herbivore.cpp
│   ├── main.cpp
│   ├── Obstacle.cpp
│   ├── Plant.cpp
│   ├── Predator.cpp
│   ├── Renderer.cpp
│   ├── SimulationEngine.cpp
│   └── StatisticsCollector.cpp
├── CMakeLists.txt
├── config.ini
└── README.md
~~~

---

## 🧠 Architecture

The project is divided into several logical modules:

- **Entity** — base class for all objects on the field
- **Animal** — common base class for living mobile creatures
- **Plant / Herbivore / Predator / Obstacle** — concrete entity types
- **Cell** — one cell of the world
- **Grid** — the full simulation field
- **SimulationEngine** — core simulation logic
- **Renderer** — graphical SFML renderer
- **ConsoleRenderer** — text renderer for debugging
- **StatisticsCollector** — collects and saves simulation statistics
- **Config** — loads parameters from the configuration file

---

## ⚙️ Configuration

All major parameters are stored in `config.ini`.

Examples of configurable parameters:

- window size
- field size
- cell size
- initial number of plants, herbivores, predators, and obstacles
- plant growth probability
- animal energy parameters
- thread count
- simulation speed
- environment mode
- statistics file name

### Example

~~~ini
grid_width=120
grid_height=90
cell_size=8

initial_plants=1500
initial_herbivores=120
initial_predators=40
initial_obstacles=500

plant_growth_probability=0.03
thread_count=4
tick_delay_ms=60
environment_mode=normal
stats_output=simulation_stats.csv
~~~

---

## 🎮 Controls

### Before launching the graphical window

The program shows a simple console menu where the user can choose:

- environment mode
- number of threads
- delay between ticks
- number of ticks for statistics

### During simulation

| Key | Action |
|-----|--------|
| `Space` | Pause / resume |
| `R` | Restart simulation |
| `S` | Save statistics |
| `Up` | Increase speed |
| `Down` | Decrease speed |

---

## 🖥 Visualization

The graphical window uses the following color mapping:

| Color | Meaning |
|-------|---------|
| Dark / Black | Empty cell |
| Green | Plant |
| Blue | Herbivore |
| Red | Predator |
| Gray | Obstacle |

The window title displays:

- current tick
- number of plants
- number of herbivores
- number of predators
- number of obstacles
- number of threads
- current delay between ticks

---

## 📊 Statistics

The program can save statistics into a CSV file.

Saved columns:

- `tick`
- `plants`
- `herbivores`
- `predators`
- `obstacles`

This allows:

- tracking ecosystem dynamics
- comparing different environment modes
- plotting graphs in external tools
- using the data in the final report

---

## ⚡ Multithreading

The project includes multithreaded sections:

- plant growth processing
- parallel counting of entity types

Technologies used:

- `std::thread`
- `std::mutex`

This makes the project closer to the requirements of a course work involving parallel execution and synchronization primitives.

---

## 🛠 Build Requirements

- **C++20**
- **CMake**
- **SFML 2.5.1**
- **MSVC / Visual Studio Build Tools**
- **Windows x64**

---

## 🚀 Build Instructions

### 1. Create a build directory

~~~bat
mkdir build
cd build
~~~

### 2. Configure the project

~~~bat
cmake ..
~~~

### 3. Build

~~~bat
cmake --build . --clean-first
~~~

### 4. Run

~~~bat
Debug\ecosystem_sim.exe
~~~

---

## 📦 Dependencies

This project uses:

- **SFML** — graphics and window management
- **STL** — containers and utility classes
- **std::thread / std::mutex** — parallel computations and synchronization

---

## 🎓 What the Project Demonstrates

This project demonstrates:

- object-oriented programming in C++
- simulation of a dynamic multi-agent system
- graphical visualization of a cellular world
- modular project organization
- multithreading and synchronization
- configuration without recompilation
- statistics collection and export
