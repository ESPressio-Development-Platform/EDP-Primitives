# EDP-Primitives Dependency Graph

> Revalidated: 2026-09-29 — canonical execution-domain scope vocabulary adds no dependency edge.

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
- `Provider`, `Offer`, `Contract`, `Composition`, and `Architecture` vocabulary used by Primitive family runtime integration.

## Explicit non-dependencies

EDP-Primitives intentionally has **no** production dependency on:

- any Transport or radio repository;
- EDP-Serialisation;
- EDP-Command;
- Event or State family repositories;
- EDP-Memory;
- EDP-Threading;
- Arduino, ESP-IDF, or another platform SDK.

Transport directionality and cross-domain bindings are outside the Primitive family plan. Integration libraries express their own compile-time declarations and satisfy family-specific contracts through normal System Composition.

## Runtime dependency behaviour

The library introduces no runtime dependency resolver, registry, discovery mechanism, heap allocator, mutable topology or provider ownership.

`Topology::Composition` is an ordinary EDP-System Composition. Application Bootstrap owns concrete provider objects. Once complete application initialization succeeds, the resolved application topology is immutable for that runtime instance.
