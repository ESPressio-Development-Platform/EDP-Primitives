# EDP-Primitives Stage 1 Architecture

## Purpose

EDP-Primitives is the common layer beneath concrete Primitive families such as future Command, Event, and State repositories.

Its job is to provide stable family-neutral compile-time contracts. It must not absorb family behaviour or create a parallel runtime architecture framework.

## Dependency direction

```text
EDP-System
    ^
    |
EDP-Primitives
    ^
    |
future Primitive-family repositories
```

There is no reverse dependency from EDP-Primitives to any family repository.

## Identity layers

Universal semantic Type identity is owned by EDP-System.

Primitive family identity is owned here:

```text
PrimitiveFamilyIdentifier
+-----------------------+---------+
| Type Authority 24-bit | family  |
|                       | 8-bit   |
+-----------------------+---------+
```

Authority zero and local family zero are invalid.

A semantic Primitive directly declares its universal `System::TypeIdentifier` and its nested `Family` Type. Neither value is stored per runtime instance.

## Family-owned deployment declarations

Topology accepts opaque family-owned declaration Types.

The only common requirement is:

```cpp
using Family = /* Primitive family tag */;
```

EDP-Primitives groups all declarations by Family before invoking the family's canonical Planner. The common layer does not interpret family-specific capacities or policy syntax.

## Planner contract

Each family tag contains exactly one canonical association:

```cpp
using Planner = FamilyPlanner;
```

The Planner receives:

1. the family Type;
2. every family-owned deployment declaration for that family;
3. the application's static `TransportSet`.

It emits one `FamilyPlan` containing:

- the represented Family;
- exactly one canonical runtime Provider Type;
- unique deployed Primitive Types;
- common deployment binding declarations;
- a decomposed bounded `ResourcePlan`.

The public `FamilyPlannerFor` concept describes this extension boundary.

## Binding normalization

Common declaration vocabulary consists of:

- `Inbound<Primitive, Transport>`;
- `Outbound<Primitive, Transport>`;
- `Bidirectional<Primitive, Transport>`;
- `AllTransports`.

Normalization:

1. expands `AllTransports` over the exact static `TransportSet`;
2. expands `Bidirectional` into separate inbound and outbound bindings;
3. rejects unconfigured concrete transports;
4. rejects duplicate explicit bindings;
5. ensures every binding targets a Primitive deployed by that family plan;
6. invokes `Family::Planner::BindingEligible<Primitive, Transport, Direction>` for each explicit binding.

No binding is synthesized when none is declared. Local-only is therefore the default.

## Serialisability boundary

Serialisability is an intrinsic compile-time Type capability but does not belong to Stage 1 EDP-Primitives.

The common layer does not depend on EDP-Serialisation and does not equate serialisable with transmissible.

A family Planner can consume whichever future Serialisation contract is appropriate and reject an ineligible requested binding through `BindingEligible`.

A serialisable Primitive with no deployment binding remains local and incurs no transport runtime requirement from EDP-Primitives.

## Resource planning

The common library does not impose `MaximumInstances<T>`.

A family Planner emits one or more semantic resource dimensions:

```cpp
ResourcePlan<
    ResourceRequirement<QueueSlots, 16U>,
    ResourceRequirement<PayloadBytesInternalRam, 4096U>
>
```

The resource tag itself carries the semantic distinction. If alignment, placement, ownership, or lifetime differ, the family must represent those as distinct dimensions rather than collapsing them into a misleading total.

Topology retains one independent resource plan per family in `ResourcePlanSet`.

## Composition integration

EDP-Primitives defines one focused Composition Domain:

```cpp
Primitives::Composition::Domain
```

and one family-parameterized exclusive capability:

```cpp
Primitives::Composition::FamilyRuntime<TFamily>
```

Every normalized family plan supplies exactly one Provider offering its corresponding capability.

Topology constructs:

```cpp
using Composition = System::CompositionFramework::Composition<
    Primitives::Composition::Domain,
    FamilyProviderA,
    FamilyProviderB
>;
```

That Composition is used directly inside normal EDP-System `Architecture<...>`.

Topology itself is metadata. It is not another Composition framework and owns no provider instances.

## Runtime ownership

Application Bootstrap owns concrete provider objects.

EDP-Primitives does not provide:

- service location;
- runtime discovery;
- runtime provider matching;
- static-registration objects;
- hidden singleton ownership;
- dynamic allocation;
- runtime topology mutation.

## Determinism

Every operation is based on compile-time Type order:

- family order is first declaration order;
- declarations remain in application order within each family;
- transport expansion follows `TransportSet` declaration order;
- normalized binding order is deterministic;
- resource plans preserve Planner declaration order;
- family runtime providers appear in normalized family order.

There is no unordered runtime data structure.
