# Build, Test and Source Map

EDP-Primitives requires C++20.

## Source layout

```text
src/
├── ESPressio_Primitives.hpp
└── primitives/
    ├── Deployment.hpp
    ├── FamilyPlan.hpp
    ├── PrimitiveComposition.hpp
    ├── PrimitiveFamilyIdentifier.hpp
    ├── PrimitiveType.hpp
    ├── Primitives.hpp
    ├── ResourcePlan.hpp
    ├── Topology.hpp
    └── TypeList.hpp
```

## Host validation

With sibling EDP-System:

```bash
python3 tests/run_tests.py
```

or:

```bash
EDP_SYSTEM_ROOT=/path/to/EDP-System python3 tests/run_tests.py
```

## Positive coverage

The main host test verifies:

- exact family identifier width and components;
- Primitive family/type concepts and readers;
- family declaration grouping;
- AllTransports and Bidirectional expansion;
- independent outbound exposure;
- resource-plan retention;
- generated Primitive Composition;
- direct System Architecture compatibility.

## Compile-fail coverage

Expected failures currently guard:

- duplicate configured transport Types;
- duplicate normalized bindings;
- family Planner binding rejection;
- binding a Primitive omitted from the family plan.

## Demo matrix

`demos/topology-basic` contains:

- Arduino IDE;
- PIOArduino Arduino;
- PIOArduino ESP-IDF.
