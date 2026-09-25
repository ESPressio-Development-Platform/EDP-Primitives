# Implementation

## Topology pipeline

Given:

```cpp
Topology<
    FamilyOneDeclarationA,
    FamilyOneDeclarationB,
    FamilyTwoDeclaration
>
```

the implementation performs deterministic compile-time stages.

### 1. Validate application declarations

Every Type must satisfy `FamilyDeploymentDeclaration`.

### 2. Discover family order

Families are collected from deployment declarations and de-duplicated while preserving first occurrence.

### 3. Group declarations

Every family receives all of its declarations, preserving application order.

### 4. Invoke canonical Planner

`Family::Planner` is invoked once with:

- the Family;
- the complete family declaration `TypeList`.

The output must satisfy `FamilyPlannerFor` and emit a canonical `FamilyPlan`.

### 5. Validate family plan

Checks include:

- plan Family matches invocation Family;
- PrimitiveTypes is a TypeList;
- every Primitive belongs to the represented Family;
- Primitive Types are unique inside the family plan;
- runtime Provider supplies the exact `FamilyRuntime<Family>` capability;
- resource output is a common `ResourcePlan`.

### 6. Build global Primitive view

Primitive lists are flattened across FamilyPlans.

The topology rejects:

- the same Primitive C++ Type appearing more than once;
- distinct Primitive Types carrying the same universal `System::TypeIdentifier`.

### 7. Retain family resources

Each FamilyPlan's decomposed ResourcePlan is retained independently through `ResourcePlanSet`. No universal total is inferred.

### 8. Build Composition

Exactly one runtime Provider Type is selected by each FamilyPlan.

Identical Provider Types are de-duplicated after family validation, so one concrete Provider may intentionally satisfy multiple family runtime capabilities and appears once in:

```cpp
System::CompositionFramework::Composition<
    Primitives::Composition::Domain,
    ...
>
```

## No second normalization layer

Validated FamilyPlan output is already canonical. There is no `NormalizedFamilyPlan` stage because EDP-Primitives performs no family-neutral Transport binding expansion after Planner output.

## Runtime footprint

Topology itself has no runtime object and creates no hidden registration object, heap allocation, mutex, Provider instance, service locator or freeze flag.
