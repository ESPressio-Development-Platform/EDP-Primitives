# Compiler Definitions and Conditional Compilation

EDP-Primitives defines no repository-specific compiler-definition configuration surface.

Architecture is configured entirely through C++20 Types and templates:

- Primitive family Types;
- semantic Primitive Types;
- family-owned deployment declarations;
- canonical family Planner output;
- bounded family resource declarations;
- System Composition provider Types.

No feature macro is required for current EDP-Primitives behaviour.

Transport/integration configuration is outside EDP-Primitives and therefore introduces no Primitive-owned compiler definition.

Platform/framework macros supplied externally by a compiler or SDK do not become EDP-Primitives configuration knobs merely because a demo build sees them.
