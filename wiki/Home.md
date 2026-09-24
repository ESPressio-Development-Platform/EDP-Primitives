# EDP-Primitives Developer Wiki

EDP-Primitives is the family-neutral compile-time foundation beneath ESPressio Primitive families.

It owns Primitive family identity, semantic Primitive classification, deployment vocabulary, family Planner extension contracts, bounded resource-plan vocabulary, topology normalization, and the focused Primitive Composition Domain.

It deliberately does **not** own Command, Event, State, Serialisation, transport implementations, runtime discovery, provider lifetime, or dynamic allocation.

## Primary public entry point

```cpp
#include <ESPressio_Primitives.hpp>
```

## Developer map

- [Architecture](Architecture) — subsystem boundaries and dependency direction.
- [Public API](Public-API) — supported consumer and family-extension vocabulary.
- [Internal API](Internal-API) — implementation-only normalization machinery.
- [Implementation](Implementation) — topology and planner execution flow.
- [Resources / Lifecycle / Concurrency](Resources-Lifecycle-Concurrency) — runtime/resource semantics.
- [Build / Test / Source](Build-Test-Source) — source layout and validation.
- [Dependency Contracts](Dependency-Contracts) — exact Stage 1 dependency boundary.
- [Reference Index](Reference-Index) — declaration-level source reference.
