# EDP-Primitives Public API

## Entry point

```cpp
#include <ESPressio_Primitives.hpp>
```

Narrow headers under `src/primitives/` may also be included directly when only one contract is required.

## Identity

### `PrimitiveFamilyIdentifier`

Exact four-byte family identity.

Construction requires:

- strong `System::TypeAuthorityIdentifier`;
- explicit non-zero `std::uint8_t` family-local value.

The Type exposes packed `Value()`, strong `Authority()`, `LocalValue()`, `IsZero()`, `IsValid()`, explicit Boolean conversion, equality, inequality, and deterministic ordering.

### `PrimitiveFamilyType<T>`

Validates a family tag containing:

- exact constant-expression `PrimitiveFamilyIdentifier Identifier`;
- non-zero Authority and local family value;
- nested canonical `Planner` Type.

### `PrimitiveType<T>`

Requires:

- `System::IdentifiedType<T>`;
- nested `Family`;
- `PrimitiveFamilyType<Family>`.

### Readers

- `PrimitiveFamilyOf<T>`;
- `PrimitiveFamilyIdentifierOf<T>`.

## Deployment

### `TransportSet<T...>`

Static concrete transport set. Duplicate Types and `AllTransports` itself are rejected.

### `Inbound`, `Outbound`, `Bidirectional`

Compile-time binding declarations. `Bidirectional` is sugar only.

### `AllTransports`

Compile-time transport selector expanded over the current `TransportSet`. It is not retained as a normalized binding.

### `TransportBinding`

Explicit normalized binding containing Primitive Type, concrete transport Type, and `DeploymentDirection`.

## Resource planning

### `ResourceRequirement<Tag, Capacity>`

One bounded semantic resource dimension.

### `ResourcePlan<...>`

One family's decomposed bounded requirements. Duplicate resource tags are rejected.

### `ResourcePlanSet<...>`

Topology-level aggregation retaining each family's plan independently.

## Planner extension

### `FamilyDeploymentDeclaration<T>`

Requires a nested valid `Family` Type.

### `FamilyPlan<...>`

Canonical family Planner output.

### `FamilyPlannerFor<...>`

Public maintained concept describing the canonical Planner `Plan` alias for one concrete planning input.

The Planner must additionally expose `BindingEligible<Primitive, Transport, Direction>` for every emitted normalized binding.

## Composition

### `Primitives::Composition::Domain`

Focused EDP-System Composition Domain for Primitive family runtimes.

### `Primitives::Composition::FamilyRuntime<TFamily>`

Exclusive capability associated with one Primitive family.

### `Primitives::Composition::FamilyRuntimeProvider<TProvider, TFamily>`

Predicate validating the runtime provider relationship.

## Topology

```cpp
Topology<
    TransportSet<TransportA, TransportB>,
    FamilyOwnedDeploymentA,
    FamilyOwnedDeploymentB
>
```

Important outputs:

- `Transports`;
- `Deployments`;
- `Families`;
- `FamilyPlans`;
- `NormalizedFamilyPlans`;
- `PrimitiveTypes`;
- `Bindings`;
- `Resources`;
- `Composition`.

All output is compile-time metadata.
