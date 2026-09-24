# src/primitives/Topology.hpp

**Primary classification:** PUBLIC TOPOLOGY API with PRIVATE IMPLEMENTATION normalization pipeline

**Source baseline:** `4aba3b000bfc896f243d11e19b724bf3da12287e`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Primitives/blob/4aba3b000bfc896f243d11e19b724bf3da12287e/src/primitives/Topology.hpp)

## `Topology<TTransportSet, TDeployments...>`

Compile-time application Primitive topology.

The first template argument must be `TransportSet`. Remaining Types must satisfy `FamilyDeploymentDeclaration`.

### Public outputs

#### `Transports`

Static configured transport set.

#### `Deployments`

Original family-owned deployment declarations in application order.

#### `Families`

Unique represented families in first-declaration order.

#### `FamilyPlans`

Canonical Planner output, one per family.

#### `NormalizedFamilyPlans`

Family plans after common binding expansion and eligibility validation.

#### `PrimitiveTypes`

Flattened deployed Primitive Types across all families. Cross-family duplicate Types are rejected.

#### `Bindings`

Flattened explicit normalized TransportBindings. Cross-family duplicates are rejected.

#### `Resources`

`ResourcePlanSet` retaining one independent decomposed resource plan per family.

#### `Composition`

Ordinary EDP-System Composition built from exactly one runtime Provider Type per family.

The generated Composition must itself report `IsValid`.

## Private pipeline

### `FilterFamilyDeclarations`

Collects all application declarations belonging to one Family.

### `DeploymentFamily`

Extracts one declaration's nested Family.

### `DeploymentFamilies`

Builds the unique family list.

### `InvokeFamilyPlanner`

Validates `FamilyPlannerFor`, invokes the direct `Family::Planner`, and validates canonical FamilyPlan output.

### `BuildFamilyPlans`

Invokes each family once with its complete grouped declaration set.

### `ValidateBinding`

Requires the Planner's `BindingEligible<Primitive, Transport, Direction>` contract and rejects false eligibility.

### `ValidateBindings`

Applies binding validation to a complete normalized list.

### `BindingsTargetDeployedPrimitives`

Ensures every explicit binding refers to a Primitive in the same family plan's deployed Primitive list.

### `NormalizedFamilyPlan`

Internal normalized representation retaining Family, RuntimeProvider, PrimitiveTypes, explicit Bindings and Resources.

### `NormalizeFamilyPlan`

Expands common deployment sugar, rejects duplicate bindings, checks target deployment membership and applies Planner eligibility.

### `NormalizeFamilyPlans`

Normalizes all family plans.

### `PlanPrimitiveTypes` / `PlanBindings`

Projection helpers for topology-wide flattening.

### `FlattenPlanLists`

Deterministically concatenates projected TypeLists.

### `MakePrimitiveComposition`

Builds the normal EDP-System Composition from family runtime Providers. Identical Provider Types are de-duplicated after each family plan has independently validated the exact FamilyRuntime capability it requires.

### `MakeResourcePlanSet`

Retains independent FamilyResources entries without inferring a universal total.
