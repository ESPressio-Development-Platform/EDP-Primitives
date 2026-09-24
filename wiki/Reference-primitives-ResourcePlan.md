# src/primitives/ResourcePlan.hpp

**Primary classification:** PUBLIC API with PRIVATE IMPLEMENTATION validation helpers

**Source baseline:** `f72501ec19ceb95295c4586ba74488acfa9ad9c9`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Primitives/blob/f72501ec19ceb95295c4586ba74488acfa9ad9c9/src/primitives/ResourcePlan.hpp)

## `ResourceRequirement<TResource, TCapacity>`

One bounded semantic resource dimension.

Public members:

- nested `Resource` tag Type;
- static `Capacity`.

The common layer assigns no byte/allocation meaning to the tag.

## `ResourcePlan<TRequirements...>`

One family's decomposed bounded resource requirements.

Requirements must be `ResourceRequirement` declarations.

Duplicate resource tags are rejected so one semantic dimension cannot silently acquire conflicting capacities.

Public members:

- `Requirements` TypeList;
- `Count`.

## `FamilyResources<TFamily, TPlan>`

Associates one Primitive family with its independent ResourcePlan.

Public members:

- nested `Family`;
- nested `Plan`.

## `ResourcePlanSet<TFamilyResources...>`

Topology-level aggregation preserving each family's plan independently.

Public members:

- `Families` TypeList;
- `Count`.

## Private implementation

`ResourceTag`, `ResourceRequirementTraits`, and related checks exist only to validate common resource vocabulary. They are not family extension points.
