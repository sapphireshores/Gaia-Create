# Gaia-Create

A high-performance incremental compilation tool and offline academic integrity guard written from scratch in C++17. Designed specifically for student development environments, Gaia-Create serves as a zero-configuration, sub-millisecond alternative to heavy build utilities like GNU Make or CMake for tracking localized sandbox architectures.

---

## Project Uniqueness and Core Problem

Students face a recurring friction point when building academic projects: choosing between configuration-intensive tools like CMake or running repetitive, manual compilation commands that waste compute cycles and constantly clear object caches. 

Gaia-Create addresses this by acting as an intelligent compiler router. It maps project layouts, verifies native operating system file modification timestamps, and utilizes a localized, private AI review pipeline. The entire system operates completely offline, ensuring project files, local data strings, and code logic never leave your host machine.

---

## Key Features

* Incremental Build System: Evaluates file modification metadata recursively using std::filesystem::last_write_time. If a source file or its mapped header components have not changed since the last invocation, compilation is skipped.
* Topological Sorting and Cycle Resolution: Constructs an internal dependency adjacency list and implements a 3-state Depth-First Search algorithm (0: Unvisited, 1: Visiting, 2: Processed) to catch and abort on circular #include blocks before the host compiler enters an infinite loop.
* Offline Academic Integrity Review: Processes source logic using local Large Language Models (LLMs) to identify structural plagiarism vectors, robotic comment symmetry, and over-engineered patterns.
* Native Sockets Bridge: Communicates with local background processes using direct, custom TCP network streams rather than relying on inline shell scripting or external utilities.
* Terminal UI Markdown Decorator: Parses raw markdown text into cleanly formatted terminal outputs using raw ANSI color codes without third-party user interface frameworks.
* Automated Workspace Sweep: Maps directories recursively to safely unlink stale object files (.o) and build clutter.

---

## System Architecture Flow

[Target Folder Input] 
         │
         ▼
[Recursive File Parser] ──► (Scans #include preprocessor tokens)
         │
         ▼
[3-State DFS Dependency Cycle Filter]
         │
         ├──► State 1 (Visiting) ──► Circular Reference Trapped (Aborts Gracefully)
         └──► State 2 (Processed) ─► Generates Safe Topological Order Layout
         │
         ▼
[Timestamp Metadata Evaluation] ──► Forwards ONLY stale modules to system shell

---

## Directory Structure

gaia-create/
├── include/
│   └── scan.h          # Component blueprints and class definitions
├── src/
│   ├── main.cpp        # CLI flag router and execution sequencer
│   └── scan.cpp        # Graph math, timestamp tracking, and native AI socket stream
├── test/               # Validation sandbox folder for nested subdirectories
├── .gitignore          # Production repository configuration filter
├── Makefile            # High-optimization strict compilation script
└── README.md           # Engineering documentation

---

## Environment Setup and Installation

### Prerequisites
Ensure your Linux distribution contains an up-to-date toolchain supporting the C++17 standard:
sudo apt update
sudo apt install build-essential curl python3 -y

### Build Instructions
Clone your repository and compile the optimized execution binary:
make

### Global Accessibility
To execute the binary from any directory across your local filesystem, copy it directly to your system path:
sudo cp gaia-create /usr/local/bin/

---

## Local AI Review Suite Setup

The AI review pipeline runs entirely on your local machine via a secure network payload channel, requiring zero cloud APIs or external internet access.

### 1. Initialize Ollama
Download and execute the open-source local inference engine:
curl -fsSL https://ollama.com/install.sh | sh

### 2. Download the Model Core
Pull down the optimized 1-Billion parameter Llama framework. This ensures swift token-generation loops on standard hardware:
ollama run llama3.2:1b

Note: Once the download completes and displays the interactive chat prompt >>>, type /exit and press Enter. The service will remain active in the background listening on port 11434.

---

## CLI Command Reference

Gaia-Create uses an explicit, flag-driven interface. Execute commands by providing a task flag followed by the target project path:

# Display all recursively discovered source files and header targets
gaia-create --list /path/to/your/target/project

# Trace internal includes, map compile queues, and process an incremental build
gaia-create --build /path/to/your/target/project

# Run the local AI review audit to scan for plagiarism structures and style tips
gaia-create --review /path/to/your/target/project

# Recursively clear out compiled .o build artifacts
gaia-create --clean /path/to/your/target/project

---

## Performance Telemetry Sample

### Cache Hit Incremental Build Performance
  [Up-to-Date] math_utils.cpp
  [Up-to-Date] string_utils.cpp
  [Up-to-Date] main.cpp

  Gaia-Create Performance Profile Metrics:
  ========================================================
  • Total Build Time Processing : 0.42 ms
  • Targets Recompiled          : 0
  • Targets Skipped (Cache Hit) : 3
  ========================================================
  Incremental compilation saved 100% of standard build times.