# EDP-Primitives Dependency Graph

> Revalidated: 2026-10-01 — Stage C1 makes `Serialisation::SerialisableType` a universal Primitive invariant.

## Library Dependency Tree

```text
EDP-Primitives
├── EDP-System [mandatory direct]
└── EDP-Serialisation [mandatory direct, compile-time qualification]
    └── lower Serialisation dependencies [transitive]
```

## Mandatory direct dependencies

### EDP-System

EDP-System owns universal Type/Field identity, `SchemaType<T>`, compile-time Composition, and the Provider/Offer/Contract/Architecture vocabulary used by Primitive family runtime integration.

### EDP-Serialisation

EDP-Primitives consumes only the public `Serialisation::SerialisableType<T>` qualification contract. `PrimitiveType<T>` requires both `System::SchemaType<T>` and `Serialisation::SerialisableType<T>`. Codec operations, wire profiles, parser state, caller buffers and representation mechanics remain wholly owned by EDP-Serialisation.

## Explicit non-dependencies

EDP-Primitives intentionally has **no direct production dependency** on Transport/radio repositories, EDP-Command, EDP-Event, EDP-State, EDP-Memory, EDP-Threading, Arduino, ESP-IDF, or another platform SDK. Lower dependencies reached through EDP-Serialisation remain transitive and are not consumed directly by Primitives source.

## Runtime dependency behaviour

The Serialisation dependency adds no runtime resolver, registry, codec state, allocator, mutable topology or provider ownership. Primitive serialisability is a compile-time admission invariant.
