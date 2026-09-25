# Primitive Topology Basic

This demonstration exercises only the family-neutral EDP-Primitives contracts.

It declares one Primitive family, two semantic Primitive Types, one family-owned deployment declaration, one bounded resource dimension, one canonical family Planner, and one family runtime Provider.

The resulting `Topology::Composition` is inserted directly into a normal EDP-System `Architecture`.

There is deliberately no Transport, Serialisation, network, Command, Event, State, Memory, Threading, or platform-specific runtime dependency in the demonstrated Primitive architecture.

Runtime checks confirm:

- the family identifier is valid;
- exactly two Primitive Types are deployed;
- exactly one family resource plan is retained;
- exactly one runtime Provider appears in Primitive Composition.
