# src/primitives/PrimitiveFamilyIdentifier.hpp

**Primary classification:** PUBLIC API

**Source baseline:** `f72501ec19ceb95295c4586ba74488acfa9ad9c9`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Primitives/blob/f72501ec19ceb95295c4586ba74488acfa9ad9c9/src/primitives/PrimitiveFamilyIdentifier.hpp)

## `PrimitiveFamilyIdentifier`

Strong exact four-byte Primitive-family identity.

### Retained state

#### `_value`

**PRIVATE IMPLEMENTATION.** Exact packed `std::uint32_t`.

Upper 24 bits contain the System Type Authority; lower 8 bits contain the authority-local family value.

### `Size`

**PUBLIC API.** Exact retained width, `4U`.

### Construction

#### default constructor

Creates the zero Invalid/Unspecified identity.

#### `PrimitiveFamilyIdentifier(TypeAuthorityIdentifier, uint8_t)`

Constructs explicitly from strong System Authority plus authority-local family value.

The constructor does not invent or allocate identity values.

### Access

#### `Value()`

Returns the exact packed 32-bit representation.

#### `Authority()`

Reconstructs the strong three-byte `System::TypeAuthorityIdentifier`.

#### `LocalValue()`

Returns the lower eight-bit family-local value.

#### `IsZero()`

Tests the complete packed zero value.

#### `IsValid()`

Requires non-zero Authority and non-zero local family value.

#### explicit `operator bool()`

Explicit validity predicate.

### Comparison

`operator ==`, `operator !=`, and `operator <` operate on the exact packed identity.

### Invariant

A static assertion enforces exactly four retained bytes.
