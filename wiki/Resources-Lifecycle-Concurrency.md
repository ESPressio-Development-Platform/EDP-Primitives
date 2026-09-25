# Resources, Lifecycle and Concurrency

## Resource model

Every scalable family runtime resource must have a finite compile-time bound.

EDP-Primitives supplies:

```cpp
ResourceRequirement<Tag, Capacity>
ResourcePlan<...>
ResourcePlanSet<...>
```

The common layer interprets resource tags only for identity and uniqueness.

A family uses distinct tags whenever placement, alignment, ownership, lifetime or other allocation semantics differ. No universal cross-family byte total is inferred.

## Runtime provider lifetime

Topology identifies Provider Types only.

Application Bootstrap owns concrete Provider instances and their lifetimes according to EDP-System Composition/Architecture lifecycle metadata.

Successful complete-Architecture initialization freezes the resolved topology for that runtime instance. Shutdown/finalization does not make it mutable again.

## Concurrency

EDP-Primitives introduces no Worker, queue, mutex, ISR bridge, Task or scheduler.

Concurrency semantics belong to concrete family/runtime implementations and their dependencies.

## ISR scope

The common layer is compile-time/value metadata and makes no ISR-callability guarantee for family runtime operations.

## Allocation

EDP-Primitives performs no dynamic allocation and depends on neither EDP-Memory nor platform allocation APIs.

## Immutability cost

No retained freeze flag, registry or rebinding table exists. Topology immutability is structural.
