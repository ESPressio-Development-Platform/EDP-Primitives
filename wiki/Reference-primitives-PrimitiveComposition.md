# src/primitives/PrimitiveComposition.hpp

**Primary classification:** PUBLIC COMPOSITION API with PRIVATE IMPLEMENTATION inspection helpers

**Source baseline:** `f72501ec19ceb95295c4586ba74488acfa9ad9c9`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Primitives/blob/f72501ec19ceb95295c4586ba74488acfa9ad9c9/src/primitives/PrimitiveComposition.hpp)

## Namespace

`ESPressio::Primitives::Composition`

## `Domain`

Concrete EDP-System Composition Domain containing Primitive family runtime providers.

It derives from `System::CompositionFramework::Domain`.

## `FamilyRuntime<TFamily>`

Exclusive capability representing exactly one runtime provider for one valid Primitive family.

Because it is an `ExclusiveCapability`, ordinary EDP-System Composition validation rejects multiple providers for the same family runtime capability.

## `FamilyRuntimeProvider<TProvider, TFamily>`

Public predicate requiring:

- normal Provider declaration metadata;
- the Primitive Composition Domain;
- an Offer of the exact `FamilyRuntime<TFamily>`.

## Private implementation

### `ProviderDeclaration<TProvider>`

Detects the public EDP-System Provider metadata required for inspection.

### `IsFamilyRuntimeProvider<TProvider, TFamily>()`

Performs the exact Domain/capability relationship check.

No runtime provider lookup or ownership is introduced by this header.
