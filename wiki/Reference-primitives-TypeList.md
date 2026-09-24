# src/primitives/TypeList.hpp

**Primary classification:** PUBLIC COMPILE-TIME VOCABULARY with PRIVATE IMPLEMENTATION helpers

**Source baseline:** `f72501ec19ceb95295c4586ba74488acfa9ad9c9`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Primitives/blob/f72501ec19ceb95295c4586ba74488acfa9ad9c9/src/primitives/TypeList.hpp)

## `TypeList<TTypes...>`

Deterministic compile-time Type list used by public Primitive extension contracts.

Public members:

- `Count`;
- `IsEmpty`;
- `Contains<TType>`.

The list owns no runtime state.

## Private implementation

### `ConcatTypeLists`

Concatenates two TypeLists preserving order.

### `AppendUniqueType`

Appends a Type only when not already represented.

### `UniqueTypeList`

Removes duplicates while preserving first occurrence.

### `HasDuplicateTypes`

Compares original and unique counts to report duplicate Types.

These helpers support normalization and are not external extension points.
