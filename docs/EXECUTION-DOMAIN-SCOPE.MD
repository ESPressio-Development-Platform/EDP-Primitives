# Execution-Domain Scope

## Purpose

`EDP-Primitives` owns the canonical family-neutral compile-time vocabulary used when a Primitive-family operation distinguishes local execution, remote execution, or independent execution in both domains.

The vocabulary is deliberately policy-only. It does not own transport bindings, routing, dispatch orchestration, expiry, rollback, quorum, fallback, or family-specific result semantics.

## Public vocabulary

The canonical Types live under `ESPressio::Primitives::ExecutionDomain`:

- `LocalOnly` — selects only the local execution domain;
- `RemoteOnly` — selects only an external remote execution domain;
- `LocalAndRemote` — selects independent operation in both domains;
- `Scope<TScope>` — accepts exactly those three scope Types, including cv/ref-qualified forms.

The Types are empty compile-time policy tags and therefore add no runtime state or dynamic memory requirement.

## Family re-export

Primitive-family repositories may re-export these canonical Types in their own namespace when doing so preserves an unambiguous and consistent family-facing API. A re-export must remain the same underlying Type rather than introducing a wrapper or parallel tag.

For example, `EDP-Command` and `EDP-Event` can expose `Command::LocalOnly` and `Event::LocalOnly` while both names resolve to `Primitives::ExecutionDomain::LocalOnly`.

## Ownership boundary

The common scope vocabulary states **which execution domain or domains are selected**. It does not define **how a family performs the operation**.

Family-specific behaviour therefore remains in the owning Primitive family. In particular:

- Command retains its Command-specific scoped dispatch orchestration and result model;
- Event retains its expiry/retention gates and remote-attempt semantics;
- future Primitive families may consume the same scope Types without inheriting Command or Event behaviour.

This boundary prevents repeated scope-tag definitions without turning `EDP-Primitives` into a cross-domain transport or dispatch framework.

## Determinism and lifecycle

The scope Types and concept are compile-time-only vocabulary. They allocate no memory, create no runtime objects, own no resources, and introduce no lifecycle or concurrency state.
