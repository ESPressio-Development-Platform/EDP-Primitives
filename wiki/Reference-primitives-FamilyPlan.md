# src/primitives/FamilyPlan.hpp

**Primary classification:** PUBLIC FAMILY EXTENSION API with PRIVATE IMPLEMENTATION validation helpers

**Source baseline:** `f72501ec19ceb95295c4586ba74488acfa9ad9c9`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Primitives/blob/f72501ec19ceb95295c4586ba74488acfa9ad9c9/src/primitives/FamilyPlan.hpp)

## Public API

### `FamilyDeploymentDeclaration<TDeclaration>`

Predicate requiring a family-owned declaration to expose a nested valid `Family`.

EDP-Primitives deliberately does not inspect the declaration's family-specific payload.

### `FamilyPlan<TFamily, TRuntimeProvider, TPrimitiveTypes, TBindings, TResources>`

Canonical output contract of a family Planner.

Public aliases:

- `Family`;
- `RuntimeProvider`;
- `PrimitiveTypes`;
- `Bindings`;
- `Resources`;
- marker `FamilyPlanTag`.

The template itself validates the represented Family.

### `FamilyPlannerFor<TPlanner, TFamily, TDeclarations, TTransportSet>`

Maintained public Planner-extension predicate.

For the concrete planning input, the Planner must expose:

```cpp
template<class TFamily, class TDeclarations, class TTransportSet>
using Plan = /* canonical FamilyPlan */;
```

The Planner's normalized binding eligibility surface is separately consumed by Topology:

```cpp
template<class TPrimitive, class TTransport, DeploymentDirection TDirection>
static constexpr bool BindingEligible = /* ... */;
```

## Private implementation

### `FamilyPlanTraits`

Detects canonical plan metadata.

### `PrimitiveListForFamily`

Requires unique valid Primitive Types all belonging to the planned family.

### `ResourcePlanTraits`

Detects common ResourcePlan output.

### `TypeListTraits`

Detects common TypeList output.

### `ValidateFamilyPlan`

Checks:

- canonical FamilyPlan shape;
- exact Family match;
- TypeList shape for PrimitiveTypes/Bindings;
- Primitive uniqueness and family membership;
- ResourcePlan shape;
- runtime Provider offering the exact `Composition::FamilyRuntime<TFamily>`.

Detail helpers are validators only and are not specialization points.
