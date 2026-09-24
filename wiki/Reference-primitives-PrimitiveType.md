# src/primitives/PrimitiveType.hpp

**Primary classification:** PUBLIC API with PRIVATE IMPLEMENTATION validation helpers

**Source baseline:** `f72501ec19ceb95295c4586ba74488acfa9ad9c9`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Primitives/blob/f72501ec19ceb95295c4586ba74488acfa9ad9c9/src/primitives/PrimitiveType.hpp)

## Public API

### `PrimitiveFamilyType<TFamily>`

Validates that a family Type declares:

- exact `PrimitiveFamilyIdentifier Identifier`;
- constant-expression identity;
- valid non-zero components;
- nested canonical `Planner` Type.

### `PrimitiveType<TType>`

Requires:

- `System::IdentifiedType<TType>`;
- nested `Family`;
- valid `PrimitiveFamilyType<Family>`.

### `PrimitiveFamilyOf<TType>`

Canonical compile-time Type reader for a semantic Primitive's nested Family.

### `PrimitiveFamilyIdentifierOf<TType>`

Canonical compile-time value reader for that Family's stable identifier.

## Private implementation

### `HasPrimitiveFamilyIdentifierMember<TFamily>`

Detects a directly readable family Identifier.

### `HasExactPrimitiveFamilyIdentifier<TFamily>()`

Requires the exact strong Primitive family identifier Type.

### `HasConstantPrimitiveFamilyIdentifier<TFamily>()`

Requires constant-expression identity.

### `HasPrimitiveFamilyPlanner<TFamily>`

Detects the direct canonical Planner association.

### `IsPrimitiveFamilyType<TFamily>()`

Composes family validation.

### `ReadPrimitiveFamilyIdentifier<TFamily>()`

Emits focused static-assert diagnostics and returns the direct family identifier.

### `HasPrimitiveFamilyMember<TType>`

Detects a semantic Primitive's nested Family.

### `ReadPrimitiveFamily<TType>`

Validates universal Type identity plus family classification and returns the direct Family Type.

None of the Detail declarations are external specialization points.
