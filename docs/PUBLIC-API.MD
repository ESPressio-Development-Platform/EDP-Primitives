# EDP-Primitives Public API

## Entry point

```cpp
#include <ESPressio_Primitives.hpp>
```

Narrow headers under `src/primitives/` may also be included directly when only one contract is required.

## Identity and schema

### `PrimitiveFamilyIdentifier`

Exact four-byte Primitive-family identity formed from a three-byte System Type Authority plus one authority-local family byte.

### `PrimitiveFamilyType<T>`

Validates a family tag containing a constant valid `PrimitiveFamilyIdentifier` and a canonical nested `Planner` Type.

### `PrimitiveType<T>`

Requires:

- a valid `System::SchemaType<T>`;
- a valid `Serialisation::SerialisableType<T>`;
- a nested `Family` Type;
- a valid Primitive family.

Therefore every Primitive has universal `System::TypeIdentifier` identity and an authoritative compile-time `Fields` schema. Zero-data Primitives declare `using Fields = System::FieldSet<>;`. Payload-bearing Primitives use explicit stable `System::FieldBinding`s.

`Serialisation::SerialisableType<T>` is now a universal Primitive invariant. This recursively validates the schema Field value Types while leaving semantic Type/Field identity in EDP-System.

### Readers

- `PrimitiveFamilyOf<T>`;
- `PrimitiveFamilyIdentifierOf<T>`.

## Execution-domain scope

The canonical family-neutral scope vocabulary is defined under `Primitives::ExecutionDomain`:

- `LocalOnly`;
- `RemoteOnly`;
- `LocalAndRemote`;
- `Scope<TScope>`.

These Types are compile-time policy tags only. Family-specific dispatch/orchestration and result semantics remain outside EDP-Primitives. See [Execution-Domain Scope](./EXECUTION-DOMAIN-SCOPE.MD).

## Resource planning

### `ResourceRequirement<Tag, Capacity>`

One bounded semantic family resource dimension.

### `ResourcePlan<...>`

One family's decomposed bounded requirements. Duplicate resource tags are rejected.

### `ResourcePlanSet<...>`

Topology-level aggregation retaining each represented family's resource plan independently.

## Family extension contract

### `FamilyDeploymentDeclaration<T>`

Requires a nested valid `Family`. The declaration payload remains opaque to EDP-Primitives.

### `FamilyPlan<TFamily, TRuntimeProvider, TPrimitiveTypes, TResources>`

Canonical family Planner output containing only the represented Family, canonical runtime provider, deployed Primitive Type list, and family-owned ResourcePlan.

### `FamilyPlannerFor<TPlanner, TFamily, TDeclarations>`

Maintained extension predicate requiring:

```cpp
template<class TFamily, class TDeclarations>
using Plan = /* canonical FamilyPlan */;
```

The complete family declaration set is supplied in original application order.

## Composition

- `Primitives::Composition::Domain`;
- `Primitives::Composition::FamilyRuntime<TFamily>`;
- `Primitives::Composition::FamilyRuntimeProvider<TProvider, TFamily>`.

Each family plan names exactly one provider satisfying its exclusive family-runtime capability.

## Topology

```cpp
Topology<
    FamilyOwnedDeploymentA,
    FamilyOwnedDeploymentB
>
```

Outputs:

- `Deployments`;
- `Families`;
- `FamilyPlans`;
- `PrimitiveTypes`;
- `Resources`;
- `Composition`.

All output is compile-time metadata. Topology rejects duplicate Primitive Types and universal TypeIdentifier collisions.

## Deliberate exclusions

EDP-Primitives provides no Transport set, inbound/outbound binding vocabulary, integration registry, runtime mutation API, or service locator. Cross-domain integration belongs to the owning integration domain and System Composition.
