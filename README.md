# Gaia-Create

A lightweight incremental build tool and offline code integrity guard written in C++17[cite: 4].

![C++17](https://img.shields.io/badge/C++-17-blue)
![Linux](https://img.shields.io/badge/Linux-Ubuntu-orange)
![Build](https://img.shields.io/badge/Build-Make-success)
![License](https://img.shields.io/badge/License-MIT-green)

## Problem
Standard build tools can be complex for small projects, while manual compilation is inefficient and often recompiles unchanged files, wasting computing resources[cite: 4].

## Solution
Gaia-Create is a smart incremental build tool that tracks file modifications to recompile only modified source files. It includes a static analyzer using local LLMs to detect structural patterns in code and suggest changes[cite: 4].

## Features
- [x] **Incremental Build Engine:** Tracks file dependencies via a Directed Acyclic Graph (DAG) and 3-state DFS cycle detection[cite: 4].
- [x] **Multi-Threaded Compilation:** Distributes compilation tasks across CPU cores using `std::async` and `std::future` with thread-safe logging[cite: 4].
- [x] **Block-Based Content Hashing:** Replaces flaky timestamps with true byte-stream hashing and a local `.gaia_cache` ledger to prevent redundant builds[cite: 4].
- [x] **Local AI Integration:** Interfaces via raw TCP/POSIX sockets with Ollama (Llama 3.2 1B) for offline static analysis and pattern recognition[cite: 4].
- [x] **JSON Compilation Database:** Generates standard `compile_commands.json` compilation databases for IDE tooling (Clangd, VS Code, Neovim)[cite: 4].
- [x] **Cross-Platform Support:** Preprocessor-guarded `#ifdef _WIN32` abstraction layer supporting Linux POSIX sockets and Windows Winsock[cite: 4].

## Algorithms - Complexity

 Dependency Graph Construction - O(V + E)[cite: 4]

 DFS Cycle Detection - O(V + E)[cite: 4]

 Incremental Scan - O(n)[cite: 4]

 Directory Traversal - O(files)[cite: 4]

## Comparison

| Feature | Make | CMake | Gaia-Create |
| :--- | :---: | :---: | :---: |
| Configuration | Complex | Complex | None[cite: 4] |
| Incremental Build | Yes[cite: 4] | Yes[cite: 4] | Yes[cite: 4] |
| Dependency Graph | Partial[cite: 4] | Yes[cite: 4] | Yes[cite: 4] |
| Local AI Review | No[cite: 4] | No[cite: 4] | Yes[cite: 4] |

## Installation

1. Install dependencies[cite: 4]:
- sudo apt update && sudo apt install build-essential curl python3

2. Compile the tool[cite: 4]:
- make

3. Install globally (creates a system-wide symbolic link)[cite: 4]:
- sudo rm -rf /usr/local/bin/gaia-create
- sudo ln -s "$(pwd)/gaia-create" /usr/local/bin/gaia-create

4. Setup for other projects[cite: 4]:
To use Gaia-Create for any project, simply reference the target directory in the build command. Gaia-Create automatically converts relative paths to absolute paths, ensuring consistent dependency resolution regardless of your current working directory[cite: 4].

5. Verify installation[cite: 4]:
- gaia-create --help

## Usage

Build a project[cite: 4]:

- gaia-create --build /path/to/your/project

Run AI review[cite: 4]:

- gaia-create --review /path/to/your/project

Clean build artifacts:

- gaia-create --clean /path/to/your/project

## Performance Metrics

![Build Summary](docs/output.png)

## Tech Stack
- **Language:** C++17[cite: 4]
- **Core Library:** `std::filesystem`[cite: 4]
- **Concurrency:** `std::async`, `std::future`, `std::mutex`
- **Algorithms:** DAG, 3-state DFS Topological Sort
- **Networking:** POSIX Sockets / Windows Winsock (`WS2_32`)
- **Build System:** Make[cite: 4]
- **AI Backend:** Ollama (Llama 3.2 1B)[cite: 4]

## Future Work
- Support for distributed multi-machine compilation caching.
- Abstracting build recipes to support multi-language ecosystems (Rust, Go, C).

## License
MIT License[cite: 4]
