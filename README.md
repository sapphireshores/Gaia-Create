# Gaia-Create

A high-performance smart incremental compilation tool and offline AI plagiarism guard written in C++17. Designed specifically for student development environments, Gaia-Create serves as a zero-configuration, sub-millisecond alternative to heavy build utilities like GNU Make or CMake for tracking localized sandbox architectures.

---------------------------------------------------------------------------------------------------------------------------------------------------

## Project Uniqueness and Core Problem

Students face a dilemma when building academic projects: choosing between configuration-intensive tools like CMake or running repetitive, manual compilation commands that are computationally intensive and inefficient in caching. 

Gaia-Create addresses this by:
* Mapping project layouts for optimized compiling flow.
* Verifying with OS file modification timestamps for selective recompile.
* Utilizing a localized, private AI review pipeline,that operates completely offline ensuring project files, local data strings, and code logic never leave the host.
---------------------------------------------------------------------------------------------------------------------------------------------------

## Key Features

* Incremental Build System: Evaluates file modification metadata recursively using std::filesystem::last_write_time. If a source file or its mapped header components have not changed since the last compilation,they are skipped in current cycle.
* Topological Sorting and Cycle Resolution: Constructs an internal dependency adjacency list and implements a 3-state Depth-First Search algorithm (0: Unvisited, 1: Visiting, 2: Processed) to catch and abort on circular #include blocks before the host compiler enters an infinite loop.
* Offline AI Plagiarism Review: Processes source logic using local Large Language Models (LLMs) to identify structural AI generated plagiarism patterns.
* Native Sockets Bridge: Communicates with local background processes using direct, custom TCP network streams rather than relying on inline shell scripting or external utilities.
* Terminal UI Markdown Decorator: Parses raw markdown text into cleanly formatted terminal outputs using raw ANSI color codes without third-party user interface frameworks.
* Workspace Clean: Maps directories recursively to safely unlink object files (.o) and build clutter.
---------------------------------------------------------------------------------------------------------------------------------------------------

## Directory Structure

### gaia-create
#### ├──include/
#####    └──scan.h          -Component blueprints and class definitions
#### ├──src/
#####    ├──main.cpp        - CLI flag router
#####    └──scan.cpp        - Graph math, timestamp tracking,native AI socket stream and markdown formatting
#### ├──test/               - Validation sandbox folder for nested subdirectories
#### ├──.gitignore          - Production repository configuration filter
#### ├──Makefile            - High-optimization strict compilation script
#### ├──README.md           - Documentation

---------------------------------------------------------------------------------------------------------------------------------------------------

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

---------------------------------------------------------------------------------------------------------------------------------------------------

## Local AI Review Suite Setup( Only needed for AI review module)

The AI review pipeline runs entirely on your local machine via a secure network payload channel,without cloud APIs or external internet access.

### 1. Initialize Ollama
Download and execute the open-source local inference engine:

curl -fsSL https://ollama.com/install.sh | sh

### 2. Download the Model Core
Pull down the optimized Llama framework to ensure swift token-generation loops on standard hardware:

ollama run llama3.2:1b

Note:Once the download completes and displays the interactive chat prompt >>>, type /exit and press Enter. The service will remain active in the background listening on port 11434.

---------------------------------------------------------------------------------------------------------------------------------------------------

## CLI Command Reference

Gaia-Create uses a flag-driven interface.Commands are executed by providing a task flag followed by the target project path:

### Display graph of directory components' file dependencies
gaia-create --graph /target/project/path

### Display all recursively discovered source files and header targets
gaia-create --list /target/project/path

### Trace internal includes, map compile queues, and process a smart incremental build
gaia-create --build /target/project/path

### Clear out compiled .o build artifacts
gaia-create --clean /target/project/path

### Run the local AI review audit to scan for AI plagiarism flags
gaia-create --review /target/project/path

---------------------------------------------------------------------------------------------------------------------------------------------------

## Performance Sample

### Incremental Build Performance
  [1/3] [Updated] math_utils.cpp

  [2/3] [Updated] string_utils.cpp

  [3/3] [Updated] main.cpp


  Gaia-Create Performance Profile Metrics:

   --------------------------------------------------------

  • Total Build Time Processing : 0.42 ms

  • Targets Recompiled          : 0

  • Targets Skipped (Cache Hit) : 3

   --------------------------------------------------------

  Incremental compilation saved 100% of standard build times.