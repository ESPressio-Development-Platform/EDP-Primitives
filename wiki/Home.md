# EDP-Primitives Developer Wiki

EDP-Primitives is the family-neutral compile-time foundation beneath EDP Primitive families.

It owns Primitive family identity, semantic Primitive classification, opaque family-owned deployment declarations, canonical family Planner contracts, decomposed bounded resource plans, topology aggregation, universal Primitive identity collision validation, and the focused Primitive Composition Domain.

It deliberately does **not** own Command/Event/State behaviour, Transport configuration or bindings, Serialisation, runtime discovery, provider lifetime, dynamic allocation, or mutable topology.

## Primary public entry point

```cpp
#include <ESPressio_Primitives.hpp>
```

## Core architectural rule

Primitive family planning is Transport-independent. Cross-domain integration belongs to its owning domain and is represented through normal EDP-System Composition providers/contracts.

Once the complete application Architecture initializes successfully, the resolved topology is immutable for that runtime instance.

## Developer map

- [Architecture](Architecture) — subsystem boundaries, planning flow and integration ownership.
- [Public API](Public-API) — supported consumer and family-extension vocabulary.
- [Internal API](Internal-API) — implementation-only family aggregation and validation machinery.
- [Implementation](Implementation) — topology and Planner execution flow.
- [Resources / Lifecycle / Concurrency](Resources-Lifecycle-Concurrency) — resource and immutable-topology semantics.
- [Build / Test / Source](Build-Test-Source) — source layout and validation.
- [Dependency Contracts](Dependency-Contracts) — exact dependency boundary.
- [Reference Index](Reference-Index) — declaration-level source reference.
