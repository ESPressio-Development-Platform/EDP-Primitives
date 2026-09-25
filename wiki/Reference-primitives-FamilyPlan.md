# src/primitives/FamilyPlan.hpp

**Primary classification:** PUBLIC FAMILY EXTENSION API with PRIVATE IMPLEMENTATION validation helpers

**Source baseline:** `b2dc70330de942be39e526a8646b7cd2d5e07cad`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Primitives/blob/b2dc70330de942be39e526a8646b7cd2d5e07cad/src/primitives/FamilyPlan.hpp)

## Public API

### `FamilyDeploymentDeclaration<TDeclaration>`

Requires a family-owned declaration to expose a nested valid `Family`.

The declaration's family-specific payload remains opaque to EDP-Primitives.

### `FamilyPlan<TFamily, TRuntimeProvider, TPrimitiveTypes, TResources>`

Canonical output contract of a family Planner.

Template parameters:

- `TFamily` — represented Primitive family;
- `TRuntimeProvider` — canonical Provider Type supplying that family's runtime capability;
- `TPrimitiveTypes` — TypeList of locally represented/deployed Primitive semantic Types;
- `TResources` — decomposed family ResourcePlan.

Public aliases are `Family`, `RuntimeProvider`, `PrimitiveTypes`, `Resources`, plus marker `FamilyPlanTag`.

No Transport binding metadata belongs to FamilyPlan.

### `FamilyPlannerFor<TPlanner, TFamily, TDeclarations>`

Maintained public Planner-extension predicate.

For the concrete planning input the Planner must expose:

```cpp
template<class TFamily, class TDeclarations>
using Plan = /* canonical FamilyPlan */;
```

`TDeclarations` is the complete family-owned declaration list in application order.

## Private implementation

### `FamilyPlanTraits`

Recognizes canonical FamilyPlan metadata.

### `PrimitiveListForFamily`

Requires unique valid Primitive Types, each belonging to the exact planned family.

### `ResourcePlanTraits`

Recognizes common ResourcePlan output.

### `TypeListTraits`

Recognizes common TypeList output.

### `ValidateFamilyPlan`

Validates canonical shape, exact Family match, Primitive list shape/membership/uniqueness, ResourcePlan shape, and exact `Composition::FamilyRuntime<TFamily>` provider capability.

These Detail helpers are validators only and are not external specialization points.
