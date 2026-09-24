# Primitive Topology Basic

This demonstration intentionally uses dummy transport and Primitive-family Types so it exercises only the Stage 1 common contracts.

There is no Serialisation, network, Command, Event, State, Memory, Threading, or platform-specific runtime dependency in the demonstrated Primitive architecture.

Expected normalized binding count is five:

- StartSignal inbound TransportA;
- StartSignal inbound TransportB;
- StartSignal outbound TransportA;
- StartSignal outbound TransportB;
- StopSignal outbound TransportA.
