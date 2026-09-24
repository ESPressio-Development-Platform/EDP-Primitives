# Internal API

Internal machinery lives primarily in `ESPressio::Primitives::Detail`.

It is documented for maintainability but is not a supported extension surface.

## Type-list machinery

`TypeList.hpp` provides deterministic compile-time concatenation, uniqueness and duplicate detection used by topology normalization.

## Deployment normalization

`Deployment.hpp` contains traits that identify common deployment declarations and expand:

- `AllTransports` over the configured transport set;
- `Bidirectional` into explicit inbound/outbound halves.

## Family-plan validation

`FamilyPlan.hpp` checks canonical Planner output shape, family membership, Primitive uniqueness, runtime Provider capability and ResourcePlan shape.

## Topology normalization

`Topology.hpp` contains the compile-time pipeline that:

1. extracts unique families;
2. groups declarations per family;
3. invokes one Planner per family;
4. expands and validates bindings;
5. flattens Primitive/binding views;
6. retains independent family resource plans;
7. builds the Primitive EDP-System Composition.

None of these helpers are external specialization points.
