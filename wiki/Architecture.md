# Architecture

## Role in the platform

```text
EDP-System
    ^
    |
EDP-Primitives
    ^
    |
Command / Event / State / future Primitive families
```

EDP-Primitives is not a runtime broker. It is compile-time architecture description and normalization.

## Universal Type identity

Semantic Type identity is owned by EDP-System. A Primitive Type therefore uses the same `System::TypeIdentifier` as any other identified platform Type.

Primitive-specific classification adds only:

```cpp
using Family = ExampleFamily;
```

## Primitive family identity

`PrimitiveFamilyIdentifier` is exactly four bytes:

```text
upper 24 bits : System Type Authority
lower 8 bits  : authority-local Primitive-family value
```

Authority zero and family-local zero are invalid.

## Family Planner

Each family tag names one canonical Planner through `Family::Planner`.

Topology first groups all family-owned deployment declarations by Family. The Planner receives the complete declaration set for its family and the application TransportSet, then emits one canonical `FamilyPlan`.

The Planner owns family-specific interpretation and eligibility. EDP-Primitives owns only the common normalized output contract.

## Deployment

Deployment is explicit and default-deny.

Common declarations:

- `Inbound`;
- `Outbound`;
- `Bidirectional`;
- `AllTransports`.

Normalization produces only concrete `TransportBinding` Types. No wildcard survives normalization.

## Resources

Families emit independent bounded resource dimensions through `ResourcePlan`.

EDP-Primitives does not collapse unlike resource dimensions into one byte count or one MaximumInstances abstraction.

## Composition

Each normalized family plan supplies exactly one runtime Provider offering:

```cpp
Composition::FamilyRuntime<TFamily>
```

Topology builds a normal EDP-System Composition from those Providers.

The application then places `Topology::Composition` directly inside ordinary `CompositionFramework::Architecture`.
