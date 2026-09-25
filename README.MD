# ESPressio Primitives

`EDP-Primitives` is the common compile-time foundation for ESPressio Primitive families.

It does **not** implement Command, Event, State, transport, Serialisation, runtime discovery, or a service registry. Those remain separate domains. This library owns only the family-neutral vocabulary required to identify Primitive families, classify semantic Primitive Types, describe deployment, invoke family-owned Planners, retain bounded resource plans, and produce the Primitive Domain Composition consumed by normal `EDP-System` Architecture.

## Stage 1 facilities

- `PrimitiveFamilyIdentifier` — exact packed four-byte family identity: upper 24 bits are the System Type Authority and lower 8 bits are the authority-local family value.
- `PrimitiveFamilyType<T>` — validates a family tag carrying a stable Identifier and exactly one canonical `Family::Planner`.
- `PrimitiveType<T>` — combines universal `System::IdentifiedType<T>` identity with direct family classification.
- `PrimitiveFamilyOf<T>` / `PrimitiveFamilyIdentifierOf<T>` — canonical compile-time readers.
- `TransportSet<...>` — statically configured concrete transports.
- `Inbound`, `Outbound`, `Bidirectional`, and `AllTransports` — common compile-time deployment vocabulary.
- `ResourceRequirement`, `ResourcePlan`, and `ResourcePlanSet` — decomposed bounded family resource vocabulary.
- `FamilyPlan` / `FamilyPlannerFor` — maintained cross-repository family Planner extension contract.
- `Topology` — groups family-owned deployment declarations, invokes each family Planner once, expands deployment sugar, validates binding eligibility, detects duplicate/conflicting declarations, and produces normalized output.
- `Composition::Domain` / `Composition::FamilyRuntime<TFamily>` — focused Primitive runtime Composition surface.
- `Topology::Composition` — ordinary `EDP-System` Composition suitable for direct inclusion in `CompositionFramework::Architecture`. If one Provider Type intentionally supplies multiple families' `FamilyRuntime<...>` capabilities, it is inserted into the generated Composition once while each family plan is validated independently.

## Semantic Type declaration

Primitive identity remains owned by the semantic Type:

```cpp
struct ExamplePrimitive final {
    static constexpr ESPressio::System::TypeIdentifier Identifier = /* explicit */;
    using Family = ExampleFamily;
};
```

No base class, CRTP, virtual dispatch, runtime registration object, registry, or per-instance identity state is required.

## Family Planner boundary

Each family owns exactly one Planner through:

```cpp
struct ExampleFamily final {
    static constexpr ESPressio::Primitives::PrimitiveFamilyIdentifier Identifier = /* explicit */;
    using Planner = ExamplePlanner;
};
```

Topology groups the complete deployment declaration set for each family and invokes:

```cpp
template<class TFamily, class TDeclarations, class TTransportSet>
using Plan = ESPressio::Primitives::FamilyPlan</* ... */>;
```

For every explicit normalized binding the Planner also exposes:

```cpp
template<
    class TPrimitive,
    class TTransport,
    ESPressio::Primitives::DeploymentDirection TDirection
>
static constexpr bool BindingEligible = /* family/Serialisation policy */;
```

This lets future family repositories apply Serialisation and family-specific eligibility without making `EDP-Primitives` depend on Serialisation.

## Deployment semantics

Deployment is default-deny/local-only. A Primitive has no transport exposure unless its family Planner emits an explicit binding declaration.

Inbound and outbound exposure are independent.

`Bidirectional` is compile-time sugar for explicit inbound plus outbound bindings.

`AllTransports` is compile-time sugar over the exact concrete transports in the application's `TransportSet`. It is never a runtime wildcard or discovery mechanism.

## Resource semantics

Every scalable runtime resource must remain finitely bounded by family-owned compile-time policy.

`EDP-Primitives` deliberately does not invent one universal `MaximumInstances<T>` or one aggregate byte total. Family Planners emit decomposed resource dimensions using semantic resource tags, preserving distinctions such as placement, alignment, ownership, and lifetime.

## Dependencies

```text
EDP-Primitives
└── EDP-System [mandatory]
```

Stage 1 has no production dependency on Serialisation, transports, Command, Event, State, Memory, Threading, or platform SDKs.

See [DEPENDENCY_GRAPH.MD](./DEPENDENCY_GRAPH.MD).

## Demos

The [`demos/topology-basic`](./demos/topology-basic) demonstration is supplied in all policy-required forms:

- Arduino IDE;
- PIOArduino / PlatformIO Arduino;
- PIOArduino / PlatformIO ESP-IDF.

## Validation

Host and compile-fail tests are under [`tests/`](./tests).

With sibling `EDP-System` and `EDP-Primitives` checkouts on `main`:

```bash
cd EDP-Primitives
python3 tests/run_tests.py
```

You can override the sibling System path with `EDP_SYSTEM_ROOT`.

## Version state

This repository has no released version. Package metadata remains at the common unreleased `0.1.0` development baseline; this Stage 1 work does not perform a version mutation.
