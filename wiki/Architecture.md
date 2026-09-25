# Architecture

## Role in the platform

```text
EDP-System
    ^
    |
EDP-Primitives
    ^
    |
Command / Event / State / future Primitive families
```

EDP-Primitives is compile-time family-neutral architecture metadata, not a runtime broker.

## Universal Type identity

Semantic Type identity is owned by EDP-System. A Primitive Type therefore uses the same `System::TypeIdentifier` as every other identified EDP Type and adds only a nested `Family` classification.

Topology enforces universal identity uniqueness across all deployed Primitive Types.

## Primitive family identity

`PrimitiveFamilyIdentifier` is exactly four bytes: three bytes of System Type Authority plus one authority-local family byte.

## Family Planner

Each family tag names one canonical Planner through `Family::Planner`.

Topology groups all family-owned deployment declarations by Family. The Planner receives the Family and its complete declaration set, then emits one canonical `FamilyPlan`.

The FamilyPlan contains runtime Provider, deployed Primitive Types and ResourcePlan only.

## Integration boundary

Transport directionality and bindings are not Primitive topology.

Transport/integration repositories own their compile-time binding declarations and adapter providers. Cross-domain validity is expressed through normal EDP-System Composition capabilities/contracts.

A Primitive family defines only the family-specific integration capabilities required by its semantics; EDP-Primitives does not define a universal Primitive-to-Transport binding contract.

## Resources

Families emit independent bounded resource dimensions through `ResourcePlan`. Unlike dimensions are not flattened into one byte total.

## Composition

Each FamilyPlan supplies exactly one runtime Provider offering:

```cpp
Composition::FamilyRuntime<TFamily>
```

Topology builds a normal EDP-System Composition from those Providers.

## Bootstrap and immutable topology

EDP-System Architecture validates the complete provider graph and lifecycle ordering at compile time; it does not own provider instances.

Application Bootstrap constructs providers and initializes the complete Architecture. Successful initialization is the topology freeze boundary.

After that point provider population, Primitive deployments, integration bindings and capacities remain immutable for that runtime instance. Operational state inside fixed providers may evolve normally.
