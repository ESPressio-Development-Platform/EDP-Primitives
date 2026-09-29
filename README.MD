# ESPressio Primitives

`EDP-Primitives` is the common compile-time foundation for ESPressio Primitive families.

It deliberately owns only family-neutral Primitive concepts:

- stable Primitive-family identity;
- classification of universally identified semantic Types into Primitive families;
- opaque family-owned deployment declarations;
- one canonical Planner contract per Primitive family;
- canonical family plans;
- decomposed bounded family resource plans;
- canonical family-neutral execution-domain scope tags for local, remote, and independent local+remote operation;
- the Primitive runtime Composition consumed by normal `EDP-System` Architecture.

It does **not** own Transport configuration, Transport bindings, Command, Event, State, Serialisation, runtime discovery, provider lifetime, service location, or dynamic allocation.

## Public entry point

```cpp
#include <ESPressio_Primitives.hpp>
```

## Documentation

- [Architecture](./docs/ARCHITECTURE.MD)
- [Public API](./docs/PUBLIC-API.MD)
- [Execution-Domain Scope](./docs/EXECUTION-DOMAIN-SCOPE.MD)
- [Building and Validation](./docs/BUILDING.MD)

## Semantic Primitive declaration

```cpp
struct ExamplePrimitive final {
    static constexpr ESPressio::System::TypeIdentifier Identifier = /* explicit */;
    using Family = ExampleFamily;
};
```

A Primitive Type carries universal `EDP-System` identity and names exactly one Primitive family. No base class, RTTI, runtime registration object or per-instance identity storage is required.

## Family Planner boundary

Each family names exactly one canonical Planner:

```cpp
struct ExampleFamily final {
    static constexpr ESPressio::Primitives::PrimitiveFamilyIdentifier Identifier = /* explicit */;
    using Planner = ExamplePlanner;
};
```

Topology groups the complete family-owned declaration set and invokes:

```cpp
template<class TFamily, class TDeclarations>
using Plan = ESPressio::Primitives::FamilyPlan</* ... */>;
```

The canonical plan contains only:

- the Family;
- one canonical family runtime Provider Type;
- unique deployed Primitive Types;
- a decomposed bounded `ResourcePlan`.

Transport and integration policy belong to the owning integration domains and are validated through normal System Composition.

## Topology

```cpp
using ApplicationPrimitives = ESPressio::Primitives::Topology<
    FamilyOwnedDeploymentA,
    FamilyOwnedDeploymentB
>;
```

Important compile-time outputs are:

- `Deployments`;
- `Families`;
- `FamilyPlans`;
- `PrimitiveTypes`;
- `Resources`;
- `Composition`.

Topology rejects duplicate deployed Primitive Types and also rejects distinct Primitive Types carrying the same universal `System::TypeIdentifier`.

## Resource semantics

Every scalable family resource is finitely bounded by family-owned compile-time policy.

`ResourcePlan` retains semantic resource dimensions independently rather than collapsing unlike storage, lifetime, placement or alignment requirements into one byte total.

## Composition and runtime ownership

Every represented family contributes exactly one canonical runtime provider satisfying:

```cpp
Primitives::Composition::FamilyRuntime<TFamily>
```

Identical Provider Types may satisfy more than one family runtime capability and appear only once in the generated Primitive Composition.

`Topology` itself is metadata. Application Bootstrap owns concrete provider objects and uses the normal `EDP-System` Architecture lifecycle model.

Once a complete application Architecture has initialized successfully, the resolved topology is immutable for that runtime instance. Runtime domain state may change; provider population, deployments, bindings and capacities do not.

## Dependencies

```text
EDP-Primitives
└── EDP-System [mandatory]
```

There is no production dependency on Transport, Serialisation, Threading, Command, Event, State, Memory, Arduino, ESP-IDF, or another platform SDK.

See [DEPENDENCY_GRAPH.MD](./DEPENDENCY_GRAPH.MD) for the revalidated dependency contract.

## Validation

With sibling `EDP-System` and `EDP-Primitives` checkouts:

```bash
cd EDP-Primitives
python3 tests/run_tests.py
```

The host suite uses C++20 with warnings-as-errors, executes positive contract tests, and confirms expected compile failures.

The `demos/topology-basic` example is supplied for Arduino IDE, PlatformIO Arduino, and PlatformIO ESP-IDF.

## Version state

The repository remains on its unreleased `0.1.0` development baseline. This Transport-decoupling architectural correction does not mutate the version.
