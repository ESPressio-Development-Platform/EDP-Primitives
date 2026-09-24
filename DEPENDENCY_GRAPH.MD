# EDP-Primitives Dependency Graph

> Baseline: 2026-09-24 — Stage 1 common Primitive foundation.

## Library Dependency Tree

```text
EDP-Primitives
└── EDP-System [mandatory]
```

## Mandatory dependency

### EDP-System

EDP-System supplies:

- universal `TypeAuthorityIdentifier` and `TypeIdentifier`;
- `IdentifiedType<T>` / `TypeIdentifierOf<T>`;
- the compile-time Composition Framework;
- ordinary `Provider`, `Offer`, `Contract`, `Composition`, and `Architecture` vocabulary used by the Primitive runtime-provider integration.

## Explicit non-dependencies

Stage 1 intentionally has **no** production dependency on:

- EDP-Serialisation;
- any transport repository;
- EDP-Command;
- EDP-Event;
- EDP-State;
- EDP-Memory;
- EDP-Threading;
- Arduino, ESP-IDF, or another platform SDK.

Transport Types are compile-time opaque Types to EDP-Primitives. Family Planners own binding eligibility, allowing future family libraries to consume Serialisation or other relevant capabilities without reversing those dependencies into the common Primitive foundation.

## Runtime dependency behaviour

The library introduces no runtime dependency resolver, registry, discovery mechanism, heap allocator, or provider ownership.

`Topology::Composition` is an ordinary EDP-System Composition. Application Bootstrap owns concrete runtime provider instances exactly as it does for other EDP-System Composition Domains.
