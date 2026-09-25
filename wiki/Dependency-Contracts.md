# Dependency Contracts

## Mandatory dependency: EDP-System

EDP-Primitives depends on EDP-System only.

EDP-System supplies:

- universal Type identity and the canonical `TypeIdentifierOf<T>` reader;
- Composition Domain/Capability/Provider/Contract vocabulary;
- Composition and Architecture validation used by Primitive family runtime providers.

Application Bootstrap, not EDP-Primitives, owns concrete Provider instances.

## Explicit non-dependencies

There is no production dependency on:

- Transport/radio libraries;
- Serialisation;
- Command;
- Event;
- State;
- Memory;
- Threading;
- Arduino;
- ESP-IDF.

## Family boundary

Primitive-family repositories depend on EDP-Primitives and own their family-specific deployment declarations and Planner interpretation.

A family may consume additional dependencies for its own behaviour or integration contracts. Those dependencies remain above EDP-Primitives and must not be reversed into this repository.

## Integration boundary

Cross-domain binding is represented through the owning integration repository's providers/contracts in the complete System Architecture.

EDP-Primitives neither consumes nor validates concrete Transport configuration.

## Immutability

The compile-time Primitive topology contains no runtime mutation surface. Successful complete-Architecture initialization establishes the immutable operational topology for that runtime instance.
