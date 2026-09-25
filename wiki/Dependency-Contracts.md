# Dependency Contracts

## Mandatory dependency

EDP-Primitives depends on **EDP-System only**.

System supplies universal Type identity and the Composition Framework used by Primitive runtime-provider integration.

## Explicit Stage 1 non-dependencies

There is no production dependency on:

- Serialisation;
- transport libraries;
- Command;
- Event;
- State;
- Memory;
- Threading;
- Arduino;
- ESP-IDF.

## Family boundary

Future Primitive-family repositories depend on EDP-Primitives.

A family Planner may consume additional family-relevant dependencies, such as a future Serialisation contract, to decide `BindingEligible`. That dependency remains above EDP-Primitives and must not be reversed into this repository.

## Workstream pin

The Primitive-introduction workstream validated against the matching EDP-System feature implementation before reintegration. With that implementation now on `EDP-System/main`, permanent PlatformIO/library metadata consumes EDP-System from `main`.
