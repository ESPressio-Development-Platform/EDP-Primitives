# Build, Test and Source Map

EDP-Primitives requires C++20.

## Source layout

```text
src/
├── ESPressio_Primitives.hpp
└── primitives/
    ├── FamilyPlan.hpp
    ├── PrimitiveComposition.hpp
    ├── PrimitiveFamilyIdentifier.hpp
    ├── PrimitiveType.hpp
    ├── Primitives.hpp
    ├── ResourcePlan.hpp
    ├── Topology.hpp
    └── TypeList.hpp
```

There is intentionally no Transport/deployment-binding header in the Primitive foundation.

## Host validation

With sibling EDP-System:

```bash
python3 tests/run_tests.py
```

or:

```bash
EDP_SYSTEM_ROOT=/path/to/EDP-System python3 tests/run_tests.py
```

The harness builds with C++20, `-Wall`, `-Wextra`, `-Werror`, and `-pedantic`.

## Positive coverage

The host tests verify:

- exact family identifier representation;
- Primitive family/type concepts and readers;
- complete family declaration grouping;
- one Planner invocation per represented family;
- empty topology;
- resource-plan retention;
- generated Primitive Composition;
- direct System Architecture compatibility;
- independently planned families sharing one runtime Provider Type with provider de-duplication.

## Compile-fail coverage

Expected failures guard:

- duplicate Primitive Types in one family plan;
- universal TypeIdentifier collisions between distinct deployed Primitive Types;
- runtime Providers missing the exact FamilyRuntime capability;
- duplicate semantic resource-dimension tags;
- invalid Primitive-family identity;
- family declarations missing their canonical Planner.

## Demo matrix

`demos/topology-basic` contains:

- Arduino IDE;
- PIOArduino Arduino;
- PIOArduino ESP-IDF.

The demo is Transport-independent.
