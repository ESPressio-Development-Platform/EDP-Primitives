# Public API

## Identity, schema and classification

- `PrimitiveFamilyIdentifier` — exact four-byte family identity.
- `PrimitiveFamilyType<T>` — valid Primitive-family predicate.
- `PrimitiveType<T>` — serialisable schema-bearing semantic Primitive predicate.
- `PrimitiveFamilyOf<T>` / `PrimitiveFamilyIdentifierOf<T>` — canonical family readers.

`PrimitiveType<T>` requires `System::SchemaType<T>`, `Serialisation::SerialisableType<T>`, a nested `Family`, and a valid Primitive family. Every Primitive therefore owns explicit universal Type identity and a canonical compile-time Field schema. Zero-data Primitives use `System::FieldSet<>`; payload-bearing Primitives use stable `System::FieldBinding`s.

The Serialisation constraint is universal: a schema-bearing Primitive with any unsupported Field value Type is rejected at compile time.

## Execution-domain scope

- `Primitives::ExecutionDomain::LocalOnly` — selects local-domain operation only.
- `Primitives::ExecutionDomain::RemoteOnly` — selects external remote-domain operation only.
- `Primitives::ExecutionDomain::LocalAndRemote` — selects independent operation in both domains.
- `Primitives::ExecutionDomain::Scope<TScope>` — concept accepting exactly the canonical scope Types, including cv/ref-qualified forms.

These Types carry no runtime state. Family-specific orchestration is deliberately outside this API.

## Resource planning

- `ResourceRequirement<Tag, Capacity>`;
- `ResourcePlan<...>`;
- `FamilyResources<Family, Plan>`;
- `ResourcePlanSet<...>`.

Resource tags remain family-owned and semantically distinct.

## Family extension contract

### `FamilyDeploymentDeclaration<T>`

Requires a nested valid `Family`. EDP-Primitives does not inspect family-specific payload.

### `FamilyPlan<TFamily, TRuntimeProvider, TPrimitiveTypes, TResources>`

Canonical Planner output containing Family, runtime Provider, deployed Primitive Types and ResourcePlan.

### `FamilyPlannerFor<TPlanner, TFamily, TDeclarations>`

Requires the canonical Planner alias:

```cpp
template<class TFamily, class TDeclarations>
using Plan = /* FamilyPlan */;
```

## Composition

- `Primitives::Composition::Domain`;
- `Primitives::Composition::FamilyRuntime<TFamily>`;
- `Primitives::Composition::FamilyRuntimeProvider<TProvider, TFamily>`.

## Topology

`Topology<FamilyDeployment...>` is the application compile-time Primitive entry point.

Outputs:

- `Deployments`;
- `Families`;
- `FamilyPlans`;
- `PrimitiveTypes`;
- `Resources`;
- `Composition`.

Topology rejects duplicate deployed Primitive Types and universal TypeIdentifier collisions.

## Not part of this API

There is no `TransportSet`, `Inbound`, `Outbound`, `Bidirectional`, `TransportBinding`, `DeploymentDirection`, runtime binding registry, or mutable topology API.
