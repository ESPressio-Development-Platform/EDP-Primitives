# Building and Validation

EDP-Primitives requires C++20.

## Dependencies

Stage 1 requires EDP-System only.

The validated Primitive-introduction implementation is now integrated to `main` in both EDP-System and EDP-Primitives.

No version number was changed by this workstream.

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

The suite compiles and runs the positive Stage 1 contracts and verifies that invalid topology forms fail compilation.

## Demo matrix

`demos/topology-basic` supplies:

- Arduino IDE;
- PIOArduino Arduino;
- PIOArduino ESP-IDF.

The PlatformIO configurations use the maintained PIOArduino platform baseline and consume EDP-System from `main`.

## Local validation limitation for this implementation session

The implementation agent's execution container cannot access the private/sibling Git repositories directly, so the complete repository test suite cannot be honestly reported as locally executed there.

The authoritative integration gate is therefore the normal local sibling-checkout test run. Any failures found there must be fixed before reintegration.
