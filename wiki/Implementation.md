# Implementation

## Topology pipeline

Given:

```cpp
Topology<
    TransportSet<A, B>,
    FamilyOneDeclarationA,
    FamilyOneDeclarationB,
    FamilyTwoDeclaration
>
```

the implementation performs these deterministic compile-time stages.

### 1. Validate application declarations

The first argument must be a `TransportSet`.

Every remaining Type must satisfy `FamilyDeploymentDeclaration`.

### 2. Discover family order

Families are collected from deployment declarations and de-duplicated while preserving first occurrence.

### 3. Group declarations

Every family receives all of its declarations, preserving application order.

### 4. Invoke canonical Planner

`Family::Planner` is invoked once with:

- the Family;
- the complete family declaration `TypeList`;
- the static `TransportSet`.

The result must satisfy `FamilyPlannerFor` and emit `FamilyPlan`.

### 5. Validate family plan

Checks include:

- plan Family matches invocation Family;
- deployed Primitive Types are unique;
- every Primitive belongs to the represented Family;
- runtime Provider supplies the exact `FamilyRuntime<Family>` capability;
- resource output is a common `ResourcePlan`.

### 6. Normalize bindings

`AllTransports` and `Bidirectional` are expanded.

The implementation rejects:

- unconfigured concrete transports;
- duplicate explicit bindings;
- bindings targeting Primitives not deployed in the family plan;
- bindings rejected by `Planner::BindingEligible`.

### 7. Build global normalized views

Primitive and binding lists are flattened across family plans.

Cross-family duplicates are rejected.

### 8. Build Composition

Exactly one runtime Provider Type from each family plan is inserted into:

```cpp
System::CompositionFramework::Composition<
    Primitives::Composition::Domain,
    ...
>
```

System Composition performs its ordinary Contract and exclusive-capability validation.

## Runtime footprint

Topology itself has no runtime object.

The library creates no hidden registration object, heap allocation, mutex, provider instance, or service locator.
