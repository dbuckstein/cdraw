# cdraw
C-based cross-platform lightweight rendering framework.

## Why?
The majority of rendering frameworks and tutorials seen online depend heavily on C++, when the rendering APIs themselves could easily interface with C. The C language is preferred here for its data-centric and explicit nature (which will play nicely with Vulkan), while C++ has some convenient object-oriented features (e.g. operator overloading, templates) that should be strictly optional and for convenience only.

## Project Guidelines
This section outlines the core guidelines, values and rules in the framework. The intention is that others may contribute to it as an open-source project, but only if following the standards for cleanliness.

### Coding Standards
* The framework is designed for applications programmed in C, using strictly data structures, non-member functions and data-oriented design/architecture. The following exceptions apply: 
    * No new C++ code is allowed in framework; existing C++ is for utility and convenience only (e.g. vector and matrix operators for higher-level mathematical purposes). Commits from forks with any new C++ syntax will not be pulled; only corrections to existing C++ will be considered.
    * Some parts of the framework may be written in platform native languages or cross-platform assembly for efficiency, however the C standard should be preferred.
* All headers must be C++ friendly for consumption only (C++ can directly consume C, but not the other way around). Use (extern "C" {...}) in case a user's C++ file consumes a framework header. Also use typedefs to allow C++ structure naming conventions to be used in C (e.g. typedef struct StructureName {...} StructureName;).
* For graphics implementations, prioritize cross-platform, C-based APIs/SDKs (e.g. Vulkan, OpenGL) and their respective shading languages and/or binary formats (GLSL, SPIR-V). Platform-specific renderer implementations may or may not be available.
* All functions must be documented before implementation. Comment interfaces/prototypes with brief descriptions, member/parameter names and descriptions and validation notes, and possible return cases denoting success, warnings or failures.
* All functions should return integer types, with the exception of math functions which primarily operate on real (float or double) numbers, or real-based (vector) numbers.
* Standard C includes should appear first in source files, if at all, to prevent framework from potentially overwriting standard data types.
* Projects must treat warnings as errors; if there is any sort of issue at any time, no matter how minor, fix it!

### Repository Standards
* The main branch and primary branch for versioning and releases is called 'main', no development is allowed on this branch.
* The primary branch for development, organization and setting up release candidates is called '_dev'.
* Branches are named with a path format (e.g. 'dev/_feature' with child 'dev/feature/_subfeature) to indicate parent/child branch relationships.
* Branches may only merge with direct parent and direct child branches, with the exception of 'release' (off of '_dev') into 'main' or release patches (off of 'main') into '_dev'.
* Commits must include a brief description of what was accomplished or is being worked on, as well as a list of changes to affected files; use brief tags to summarize change details for each specific file (e.g. MOD [modified], ADD [added], DEL [deleted], REN [renamed], BUG for emergent issues or FIX for solved issues).
