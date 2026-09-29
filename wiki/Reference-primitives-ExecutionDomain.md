# src/primitives/ExecutionDomain.hpp

**Primary classification:** PUBLIC API

## Purpose

Defines the canonical family-neutral compile-time execution-domain scope vocabulary shared by Primitive families. The header owns selection policy only and deliberately does not own dispatch, routing, transport, expiry or family-specific result semantics.

## Namespace `ESPressio::Primitives::ExecutionDomain`

### `LocalOnly`

Empty compile-time policy Type selecting operation in the local Primitive execution domain only. It owns no runtime state, memory or lifecycle. Primitive families may re-export this exact Type.

### `RemoteOnly`

Empty compile-time policy Type selecting operation in an external remote Primitive execution domain only. The Type does not identify or bind any transport or remote endpoint.

### `LocalAndRemote`

Empty compile-time policy Type selecting independent operation in both local and external remote execution domains. It deliberately establishes no ordering, transaction, rollback, fallback, quorum or aggregate-success semantics.

### `Scope<TScope>`

Public concept accepting exactly `LocalOnly`, `RemoteOnly`, or `LocalAndRemote` after cv/ref removal.

**Template parameter**

- `TScope` — candidate compile-time execution-domain policy Type to classify.

The concept is intended for family APIs that need to constrain a scope argument while retaining one canonical semantic Type identity across Primitive families.

## Resource, lifetime and concurrency semantics

All declarations in this header are compile-time-only. They allocate no storage, own no resources, expose no blocking behaviour, and introduce no ISR or thread-safety contract.
