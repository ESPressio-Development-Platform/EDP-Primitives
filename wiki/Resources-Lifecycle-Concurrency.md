# Resources, Lifecycle and Concurrency

## Resource model

Every scalable family runtime resource must have a finite compile-time bound.

EDP-Primitives supplies:

```cpp
ResourceRequirement<Tag, Capacity>
ResourcePlan<...>
ResourcePlanSet<...>
```

The common layer intentionally does not interpret resource tags beyond identity and uniqueness.

A family must use distinct resource tags whenever placement, alignment, ownership, lifetime, or other allocation semantics differ.

No universal cross-family byte total is inferred.

## Runtime provider lifetime

Topology identifies Provider Types only.

Application Bootstrap owns concrete Provider instances and their lifetimes according to the normal EDP-System Composition/Architecture model.

## Concurrency

EDP-Primitives introduces no worker, queue, mutex, ISR bridge, task, or scheduler.

Concurrency semantics belong to concrete family/runtime implementations and their dependencies.

## ISR scope

The Stage 1 common layer is compile-time/value metadata and makes no new ISR-callability guarantee for future family runtime operations.

## Allocation

The Stage 1 library performs no dynamic allocation and depends on neither EDP-Memory nor platform allocation APIs.
