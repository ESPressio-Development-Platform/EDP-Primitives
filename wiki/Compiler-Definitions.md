# Compiler Definitions and Conditional Compilation

EDP-Primitives defines no repository-specific compiler-definition configuration surface.

Architecture is configured using C++20 Types and templates:

- Primitive family Types;
- semantic Primitive Types;
- family-owned deployment declarations;
- TransportSet;
- Family Planner output.

No feature macro is required for Stage 1 behaviour.

Platform/framework macros supplied externally by a compiler or SDK do not become EDP-Primitives configuration knobs merely because a demo build sees them.
