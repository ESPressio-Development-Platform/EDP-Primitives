# src/primitives/Deployment.hpp

**Primary classification:** PUBLIC API with PRIVATE IMPLEMENTATION normalization helpers

**Source baseline:** `daa6292ed43282e8f694dd06bfc3ff1e3e7f5238`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Primitives/blob/daa6292ed43282e8f694dd06bfc3ff1e3e7f5238/src/primitives/Deployment.hpp)

## Public declarations

### `DeploymentDirection`

Strong enum containing `Inbound` and `Outbound`.

### `AllTransports`

Compile-time sugar marker. It cannot appear as a configured concrete transport and does not survive normalization.

### `TransportSet<TTransports...>`

Static application transport population.

Rejects duplicate transport Types and `AllTransports`.

Public members:

- `Types` — common TypeList;
- `Count`;
- `Contains<TTransport>`.

### `Inbound<TPrimitive, TTransport>`

Common inbound exposure declaration.

### `Outbound<TPrimitive, TTransport>`

Common outbound exposure declaration.

### `Bidirectional<TPrimitive, TTransport>`

Compile-time sugar for both directions.

All three require a valid `PrimitiveType` and expose nested `Primitive` and `Transport`.

### `TransportBinding<TPrimitive, TTransport, TDirection>`

Explicit normalized binding.

Requires a valid Primitive and a concrete transport other than `AllTransports`.

Public members:

- nested `Primitive`;
- nested `Transport`;
- static `Direction`.

## Private implementation

### `TransportSetTraits`

Identifies common TransportSet Types.

### `DeploymentDeclarationTraits`

Identifies Inbound/Outbound/Bidirectional and exposes expansion metadata.

### `BindAllTransports`

Emits one explicit binding per configured transport for one direction.

### `ExpandDeploymentDeclaration`

Expands one common declaration. Rejects explicit transports absent from the TransportSet.

### `ExpandDeploymentDeclarations`

Recursively expands a TypeList while preserving deterministic declaration order.
