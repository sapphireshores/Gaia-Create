# Gaia-Create 🚀

A lightweight, high-performance incremental compilation tool and offline academic integrity guard written entirely from scratch in C++17. Designed specifically for student development environments, Gaia-Create serves as a zero-config, sub-millisecond alternative to heavy configuration utilities like GNU Make or CMake for tracking sandbox architectures.

---

## ⚡ The Inspiration & Core Problem (Project Uniqueness)

Students face a frustrating friction point when building academic projects: either spend hours dealing with bloated, configuration-intensive tools like CMake or run blind, repetitive `g++` compilation lines that waste compute cycles and constantly wipe out object caches. 

**Gaia-Create** solves this by acting as a smart compiler router. It maps deep, recursive project layouts, checks operating system file modification clocks natively, and loops a localized, completely private AI review script. **The entire platform operates 100% offline**, meaning your project files, local data strings, and code logic never leave your computer.

---

## ✨ Flagship Features

- **Smart Incremental Builds:** Evaluates high-resolution system clock metadata (`std::filesystem::last_write_time`) recursively. If a source file or its mapped header components have not been modified since the last run, it skips compiling them entirely, shaving down execution loops to sub-milliseconds.
- **Topological Sorting with Cycle Resolution:** Constructs an internal dependency Adjacency List. It implements a 3-state Depth-First Search (DFS) algorithm (`0: Unvisited`, `1: Visiting`, `2: Processed`) to instantly catch and gracefully abort on circular `#include` blocks before the host compiler freezes.
- **Offline AI Academic Integrity Guard:** An isolated review suite that processes source logic using local Large Language Models (LLMs) to flag structural plagiarism vectors, robotic comment symmetry, and over-engineered patterns before code is submitted to strict grading engines.
- **Native C++ Sockets Bridge:** Communicates with local background processes using direct, custom TCP network streams rather than relying on brittle inline shell scripting or insecure external curl utilities.
- **Terminal UI Markdown Decorator:** A built-in string decorator engine that parses raw markdown outputs into colorful, easily readable terminal text arrays using raw ANSI color vectors without relying on third-party UI framework wrappers.
- **Automated Workspace Sweep:** An automated cleanup engine that maps folders recursively to safely unlink stale object files (`.o`) and build clutter.

---

## 🛠️ System Architecture & Mechanics

### Compilation Directed Acyclic Graph (DAG) Workflow
---

## 📦 Directory Structure

```text
gaia-create/
├── include/
│   └── scan.h          # Component blueprints, structural configurations, and class limits
├── src/
│   ├── main.cpp        # CLI flag router, configuration map, and execution sequencer
│   └── scan.cpp        # Graph math, system timestamp tracking, and native AI socket stream
├── test/               # Validation sandbox folder (supports deeply nested child directories)
├── .gitignore          # Production repository configuration filter
├── Makefile            # High-optimization strict compilation script
└── README.md           # Engineering documentation portfolio
[Target Folder Input]
│
▼
[Recursive File Parser] ──► (Scans #include preprocessor tokens)
│
▼
[3-State DFS Dependency Cycle Filter]
│
├──► State 1 (Visiting) ──► 🚨 Circular Reference Trapped! (Aborts Gracefully)
└──► State 2 (Processed) ─► Generates Safe Topological Order Layout
│
▼
[Timestamp Metadata Evaluation] ──► Forwards ONLY stale modules to system shell


---

## 📦 Directory Structure

```text
gaia-create/
├── include/
│   └── scan.h          # Component blueprints, structural configurations, and class limits
├── src/
│   ├── main.cpp        # CLI flag router, configuration map, and execution sequencer
│   └── scan.cpp        # Graph math, system timestamp tracking, and native AI socket stream
├── test/               # Validation sandbox folder (supports deeply nested child directories)
├── .gitignore          # Production repository configuration filter
├── Makefile            # High-optimization strict compilation script
└── README.md           # Engineering documentation portfolio

🚀 Environment Setup & Installation
Prerequisites

Ensure your Linux machine has an up-to-date toolchain that natively supports the C++17 standard:
Bash

sudo apt update
sudo apt install build-essential curl python3 -y

Installation

Clone your repository and compile the highly optimized execution binary:
Bash

make

Making the Engine Globally Accessible

To run Gaia-Create anywhere across your local machine just like a native Linux utility, copy the final executable directly to your system bin directory:
Bash

sudo cp gaia-create /usr/local/bin/

🤖 Local AI Review Suite Setup Guide

The AI review pipeline runs entirely on your local machine via a secure network payload channel. It requires zero cloud APIs, preserving absolute data privacy and ensuring zero external network latency.
1. Initialize Ollama

Download and run the open-source local inference container engine:
Bash

curl -fsSL [https://ollama.com/install.sh](https://ollama.com/install.sh) | sh

2. Download the Fast Llama Code Core

Pull down the hyper-optimized 1-Billion parameter Llama framework. This ensures swift token-generation loops even on standard laptop hardware:
Bash

ollama run llama3.2:1b

Note: Once the download status hits 100% and displays the interactive chat prompt >>>, simply type /exit and press Enter. The service will remain alive seamlessly in the background as a local system daemon listening on port 11434.
💻 CLI Command Reference

Gaia-Create features an explicit, flag-driven interface. Execute commands by feeding a task flag followed by the target project path:
Bash

# Display all recursively discovered source files and header targets
gaia-create --list /path/to/your/target/project

# Trace internal includes, map compile queues, and process an incremental build
gaia-create --build /path/to/your/target/project

# Run the local AI review audit to scan for plagiarism structures and style tips
gaia-create --review /path/to/your/target/project

# Recursively clear out old compiled .o build artifacts
gaia-create --clean /path/to/your/target/project

⏱️ Telemetry & Real-World Profiling Logs
Cache Hit Incremental Build Performance (--build)

Running successive compilation calls validates targets against drive metadata without spawning external process forks, maximizing computing efficiency:
Plaintext

  ✨ [Up-to-Date] math_utils.cpp
  ✨ [Up-to-Date] string_utils.cpp
  ✨ [Up-to-Date] main.cpp

⏱️  Gaia-Create Performance Profile Metrics:
  ========================================================
  • Total Build Time Processing : 0.42 ms
  • Targets Recompiled          : 0
  • Targets Skipped (Cache Hit) : 3
  ========================================================
  ⚡ Zero overhead! Incremental compilation saved 100% of standard build times.

Local AI Plagiarism Audit Display Sample (--review)
Plaintext

--------------------------------------------------------
🤖 [1/3] Analyzing main.cpp ...

💡 [AI Academic Integrity Assessment]:
  • Warning: The array sorting iterator implementation looks overly optimized for a standard classroom template assignment.
  • Formatting: Comment flags display robotic, flawless structural symmetry. Consider refactoring style layouts to ensure a more authentic, human-like structure.