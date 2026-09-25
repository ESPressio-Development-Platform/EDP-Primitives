# src/primitives/Topology.hpp

**Primary classification:** PUBLIC TOPOLOGY API with PRIVATE IMPLEMENTATION aggregation/validation pipeline

**Source baseline:** `b2dc70330de942be39e526a8646b7cd2d5e07cad`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Primitives/blob/b2dc70330de942be39e526a8646b7cd2d5e07cad/src/primitives/Topology.hpp)

## `Topology<TDeployments...>`

Compile-time Primitive deployment topology.

Every template argument must satisfy `FamilyDeploymentDeclaration`.

Topology is metadata only; it allocates no runtime object and contains no Transport configuration.

### Public outputs

- `Deployments` — original family-owned declarations in application order;
- `Families` — unique represented families in first-declaration order;
- `FamilyPlans` — one canonical validated Planner result per represented family;
- `PrimitiveTypes` — flattened deployed Primitive Types;
- `Resources` — ResourcePlanSet retaining one independent plan per family;
- `Composition` — ordinary EDP-System Composition built from canonical family runtime Providers.

Topology rejects duplicate Primitive C++ Types and distinct Primitive Types sharing one universal `System::TypeIdentifier`.

## Private pipeline

### `FilterFamilyDeclarations`

Collects all application declarations belonging to one Family while preserving application order.

### `DeploymentFamily` / `DeploymentFamilies`

Extract declaration family metadata and build the unique first-occurrence family list.

### `InvokeFamilyPlanner`

Validates `FamilyPlannerFor`, invokes the direct `Family::Planner`, and validates FamilyPlan output.

### `BuildFamilyPlans`

Invokes each represented family exactly once with its complete grouped declaration set.

### `PlanPrimitiveTypes` / `FlattenPlanLists`

Project and concatenate deployed Primitive TypeLists in deterministic family order.

### `PrimitiveIdentifierUniqueAgainstV` / `UniquePrimitiveIdentifiers`

Compare canonical `System::TypeIdentifierOf<T>` values and reject topology-wide semantic identity collisions without a runtime registry.

### `MakePrimitiveComposition`

Builds the Primitive EDP-System Composition. Identical Provider Types are de-duplicated after each FamilyPlan has independently validated its required FamilyRuntime capability.

### `MakeResourcePlanSet`

Retains independent FamilyResources entries without inferring a universal total.

There is deliberately no binding normalization stage.
