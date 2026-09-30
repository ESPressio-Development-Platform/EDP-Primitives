# EDP-Primitives Architecture

## Purpose

EDP-Primitives is the common layer beneath concrete Primitive families such as Command, Event and State.

Its job is to provide stable family-neutral compile-time contracts. It must not absorb family behaviour, Transport policy, integration binding semantics, or create a parallel runtime architecture framework.

## Dependency direction

```text
EDP-System
    ^
    |
EDP-Primitives
    ^
    |
Primitive-family repositories
```

There is no reverse dependency from EDP-Primitives to a family, Transport, Serialisation or platform repository during the Stage-A prerequisite tranche.

After EDP-Serialisation exists, EDP-Primitives will add the universal compile-time `SerialisableType` constraint in the already-locked dependency direction; that later tightening does not change the System-owned schema contract described here.

## Identity and schema layers

Universal semantic Type and Field identity are owned by EDP-System.

Every semantic Primitive satisfies `System::SchemaType` and therefore directly declares:

- its universal `System::TypeIdentifier`;
- its canonical compile-time `Fields` set.

A zero-data Primitive explicitly declares `System::FieldSet<>`. Payload-bearing Primitives use stable numeric `System::FieldIdentifier`s through `System::FieldBinding`s. Member declaration order is not semantic Field identity.

Primitive family identity is owned here:

```text
PrimitiveFamilyIdentifier
+-----------------------+---------+
| Type Authority 24-bit | family  |
|                       | 8-bit   |
+-----------------------+---------+
```

A semantic Primitive additionally declares its nested `Family` Type.

Topology rejects both repeated Primitive C++ Types and distinct Primitive Types that collide on universal TypeIdentifier.

## Family-owned deployment declarations

Topology accepts opaque family-owned declaration Types.

The only common requirement is:

```cpp
using Family = /* Primitive family tag */;
```

EDP-Primitives groups all declarations by Family while preserving declaration order. It does not interpret family-specific capacities, local execution policies, or other family-owned syntax.

## Planner contract

Each family tag contains exactly one canonical association:

```cpp
using Planner = FamilyPlanner;
```

The Planner receives:

1. the family Type;
2. every family-owned deployment declaration for that family.

It emits one `FamilyPlan` containing:

- the represented Family;
- exactly one canonical runtime Provider Type;
- unique deployed Primitive Types;
- a decomposed bounded `ResourcePlan`.

There is no common post-Planner transport normalization stage. Validated FamilyPlan output is already canonical.

## Execution-domain scope ownership

EDP-Primitives owns the canonical compile-time `ExecutionDomain::LocalOnly`, `RemoteOnly`, and `LocalAndRemote` policy tags plus the `ExecutionDomain::Scope` concept. This common vocabulary is family-neutral and may be re-exported by Primitive-family repositories.

The common layer does not implement dispatch coordination or transport behaviour. Each family retains the semantics required to act upon the selected domain scope.

## Transport and integration boundary

Transport configuration and inbound/outbound bindings are deliberately outside EDP-Primitives.

Integration domains may define compile-time binding declarations and concrete adapter providers, but those relationships normalize into ordinary System Composition providers/contracts before application initialization.

Primitive families expose only the narrow integration capabilities required by their own semantics. EDP-Primitives does not provide a universal Primitive-to-Transport binding abstraction.

## Resource planning

The common library does not impose one universal maximum-instance or byte-total model.

A family Planner emits semantic resource dimensions such as:

```cpp
ResourcePlan<
    ResourceRequirement<QueueSlots, 16U>,
    ResourceRequirement<PayloadBytesInternalRam, 4096U>
>
```

If alignment, placement, ownership or lifetime differ, the family represents them as distinct dimensions.

## Composition integration

EDP-Primitives defines:

```cpp
Primitives::Composition::Domain
Primitives::Composition::FamilyRuntime<TFamily>
```

Each family plan supplies exactly one Provider offering its corresponding capability.

One Provider Type may intentionally satisfy more than one family's runtime capability. Topology de-duplicates identical Provider Types only after every family plan has independently validated the exact capability it requires.

The generated Primitive Composition is inserted directly into the complete EDP-System Architecture.

## Runtime ownership and immutability

Topology itself is metadata and owns no provider instances.

Application Bootstrap owns concrete provider objects, resolves validated non-owning relationships, and initializes the complete Architecture according to Architecture lifecycle order.

Successful completion of application initialization is the operational freeze boundary. From then on, the resolved topology is immutable for that runtime instance. Domain state may evolve; provider population, deployments, cross-domain bindings and capacities do not.

A different topology requires a newly constructed and initialized runtime instance.

## No runtime registration

EDP-Primitives provides no service location, runtime discovery, registration objects, hidden singleton ownership, dynamic allocation, or runtime topology mutation.
