# src/primitives/PrimitiveType.hpp

**Primary classification:** PUBLIC API with PRIVATE IMPLEMENTATION validation helpers

**Source baseline:** `851fde8d9d9d70dc7023b7fd99736c019696c8de`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Primitives/blob/851fde8d9d9d70dc7023b7fd99736c019696c8de/src/primitives/PrimitiveType.hpp)

## Public API

### `PrimitiveFamilyType<TFamily>`

Validates that a family Type declares:

- exact `PrimitiveFamilyIdentifier Identifier`;
- constant-expression identity;
- valid non-zero components;
- nested canonical `Planner` Type.

### `PrimitiveType<TType>`

Requires:

- `System::SchemaType<TType>`;
- nested `Family`;
- valid `PrimitiveFamilyType<Family>`.

Every Primitive is therefore structurally schema-bearing. A zero-data Primitive declares `using Fields = System::FieldSet<>;`; payload-bearing Primitives declare explicit stable `System::FieldBinding`s. Numeric FieldIdentifier is authoritative semantic Field identity.

The Stage-A prerequisite contract intentionally does not yet require `Serialisation::SerialisableType`; that becomes universal only after `EDP-Serialisation` exists.

### `PrimitiveFamilyOf<TType>`

Canonical compile-time Type reader for a semantic Primitive's nested Family. Its focused diagnostic path also requires `System::SchemaType<TType>`.

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

Validates universal schema qualification plus family classification and returns the direct Family Type.

None of the Detail declarations are external specialization points.
