# Building and Validation

EDP-Primitives requires C++20.

## Dependencies

EDP-Primitives requires EDP-System only.

No Transport, Serialisation or Primitive-family repository is required to build the foundation.

No version number is changed by the Transport-decoupling refactor; the repository remains on its unreleased development baseline.

## Host tests

With sibling checkouts:

```text
parent/
├── EDP-System/
└── EDP-Primitives/
```

run:

```bash
cd EDP-Primitives
python3 tests/run_tests.py
```

For a non-sibling System checkout:

```bash
EDP_SYSTEM_ROOT=/path/to/EDP-System python3 tests/run_tests.py
```

The suite compiles and runs positive C++20 contracts with `-Wall -Wextra -Werror -pedantic` and verifies that invalid topology forms fail compilation.

Positive coverage includes empty topology, family grouping, complete declaration delivery, Primitive identity, resource aggregation, generated Primitive Composition, System Architecture compatibility, and shared runtime Provider de-duplication.

Compile-fail coverage includes duplicate Primitive Types, duplicate universal TypeIdentifier values, invalid family runtime providers, duplicate resource dimensions, invalid family identities, and missing family Planners.

## Demo matrix

`demos/topology-basic` supplies:

- Arduino IDE;
- PIOArduino / PlatformIO Arduino;
- PIOArduino / PlatformIO ESP-IDF.

The demo exercises only Primitive family planning, bounded resources and Composition. It intentionally contains no Transport mock or binding vocabulary.

## Validation status

Repository test/build commands must be executed against the branch tip before completion is claimed. Historical validation results do not substitute for current-tip execution.
