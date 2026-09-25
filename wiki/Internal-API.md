# Internal API

Internal machinery lives primarily in `ESPressio::Primitives::Detail`.

It is documented for maintainability but is not a supported family extension surface.

## Type-list machinery

`TypeList.hpp` provides deterministic compile-time concatenation, uniqueness and duplicate detection.

## Family-plan validation

`FamilyPlan.hpp` checks canonical Planner output shape, exact Family match, Primitive family membership, Primitive-Type uniqueness, ResourcePlan shape, and runtime Provider capability.

## Topology aggregation

`Topology.hpp` performs the compile-time pipeline:

1. extract unique families in first-declaration order;
2. group complete declarations per family;
3. invoke one canonical Planner per family;
4. validate each FamilyPlan;
5. flatten Primitive Types;
6. reject duplicate C++ Primitive Types;
7. reject universal TypeIdentifier collisions;
8. retain independent family resource plans;
9. build the Primitive EDP-System Composition while de-duplicating identical Provider Types.

There is no Transport expansion or integration normalization in this library.

None of these Detail helpers are external specialization points.
