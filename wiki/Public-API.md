# Public API

## Identity and classification

### `PrimitiveFamilyIdentifier`

Strong exact four-byte family identity.

### `PrimitiveFamilyType<T>`

Valid family tag predicate.

### `PrimitiveType<T>`

Valid semantic Primitive predicate.

### Readers

- `PrimitiveFamilyOf<T>`
- `PrimitiveFamilyIdentifierOf<T>`

## Deployment

### `TransportSet<T...>`

Static concrete transport population.

### `Inbound<Primitive, Transport>`

Explicit inbound exposure.

### `Outbound<Primitive, Transport>`

Explicit outbound exposure.

### `Bidirectional<Primitive, Transport>`

Compile-time sugar for inbound plus outbound exposure.

### `AllTransports`

Compile-time sugar selecting every concrete transport in the current `TransportSet`.

### `TransportBinding<...>`

Explicit normalized Primitive/transport/direction relationship.

## Resource planning

- `ResourceRequirement<Tag, Capacity>`
- `ResourcePlan<...>`
- `FamilyResources<Family, Plan>`
- `ResourcePlanSet<...>`

## Family extension contract

### `FamilyDeploymentDeclaration<T>`

Requires a nested valid `Family`.

### `FamilyPlan<...>`

Canonical family Planner output.

### `FamilyPlannerFor<...>`

Public Planner contract predicate.

## Composition

- `Primitives::Composition::Domain`
- `Primitives::Composition::FamilyRuntime<TFamily>`
- `Primitives::Composition::FamilyRuntimeProvider<TProvider, TFamily>`

## Topology

`Topology<TransportSet<...>, FamilyDeployment...>` is the application compile-time entry point.

Important normalized outputs:

- `Families`
- `FamilyPlans`
- `NormalizedFamilyPlans`
- `PrimitiveTypes`
- `Bindings`
- `Resources`
- `Composition`
