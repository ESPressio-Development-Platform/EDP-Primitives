#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "PrimitiveType.hpp"
#include "TypeList.hpp"

namespace ESPressio::Primitives {

    /// Direction of one explicit normalized Primitive transport binding.
    enum class DeploymentDirection : std::uint8_t {
        Inbound = 0U,
        Outbound = 1U
    };


    /// Compile-time marker selecting every transport explicitly configured by one Topology.
    struct AllTransports final {};


    /// Static set of concrete transports available to one Primitive Topology.
    ///
    /// @tparam TTransports Concrete transport Types configured by the application.
    template<class... TTransports>
    struct TransportSet final {

        static_assert(
            !Detail::HasDuplicateTypes<TypeList<TTransports...>>::Value,
            "TransportSet must not contain duplicate transport Types"
        );

        static_assert(
            (!std::is_same_v<TTransports, AllTransports> && ...),
            "AllTransports is deployment sugar and cannot be configured as a concrete transport"
        );

        /// Configured transports in deterministic declaration order.
        using Types = TypeList<TTransports...>;

        /// Number of statically configured transports.
        static constexpr std::size_t Count = sizeof...(TTransports);

        /// Indicates whether the supplied transport is statically configured.
        ///
        /// @tparam TTransport Transport Type being queried.
        template<class TTransport>
        static constexpr bool Contains = Types::template Contains<TTransport>;

    };


    /// Declares inbound transport exposure for one Primitive Type.
    ///
    /// @tparam TPrimitive Semantic Primitive Type being exposed.
    /// @tparam TTransport Concrete transport Type or AllTransports sugar.
    template<class TPrimitive, class TTransport>
    struct Inbound final {

        static_assert(
            PrimitiveType<TPrimitive>,
            "Inbound requires a valid Primitive Type"
        );

        /// Primitive Type represented by this declaration.
        using Primitive = TPrimitive;

        /// Requested concrete transport or AllTransports sugar.
        using Transport = TTransport;

    };


    /// Declares outbound transport exposure for one Primitive Type.
    ///
    /// @tparam TPrimitive Semantic Primitive Type being exposed.
    /// @tparam TTransport Concrete transport Type or AllTransports sugar.
    template<class TPrimitive, class TTransport>
    struct Outbound final {

        static_assert(
            PrimitiveType<TPrimitive>,
            "Outbound requires a valid Primitive Type"
        );

        /// Primitive Type represented by this declaration.
        using Primitive = TPrimitive;

        /// Requested concrete transport or AllTransports sugar.
        using Transport = TTransport;

    };


    /// Compile-time sugar declaring both inbound and outbound exposure.
    ///
    /// @tparam TPrimitive Semantic Primitive Type being exposed.
    /// @tparam TTransport Concrete transport Type or AllTransports sugar.
    template<class TPrimitive, class TTransport>
    struct Bidirectional final {

        static_assert(
            PrimitiveType<TPrimitive>,
            "Bidirectional requires a valid Primitive Type"
        );

        /// Primitive Type represented by this declaration.
        using Primitive = TPrimitive;

        /// Requested concrete transport or AllTransports sugar.
        using Transport = TTransport;

    };


    /// One explicit normalized Primitive/transport/direction binding.
    ///
    /// AllTransports and Bidirectional declarations normalize to this representation.
    ///
    /// @tparam TPrimitive Semantic Primitive Type.
    /// @tparam TTransport Concrete statically configured transport Type.
    /// @tparam TDirection Explicit transport direction.
    template<
        class TPrimitive,
        class TTransport,
        DeploymentDirection TDirection
    >
    struct TransportBinding final {

        static_assert(
            PrimitiveType<TPrimitive>,
            "TransportBinding requires a valid Primitive Type"
        );

        static_assert(
            !std::is_same_v<TTransport, AllTransports>,
            "Normalized TransportBinding requires a concrete transport Type"
        );

        /// Primitive Type carried by this binding.
        using Primitive = TPrimitive;

        /// Concrete transport Type carrying the Primitive.
        using Transport = TTransport;

        /// Explicit normalized binding direction.
        static constexpr DeploymentDirection Direction = TDirection;

    };


    namespace Detail {

        /// Default TransportSet metadata for unrelated Types.
        ///
        /// @tparam TType Type being inspected.
        template<class TType>
        struct TransportSetTraits final {

            /// Indicates whether the inspected Type is a common TransportSet.
            static constexpr bool IsValid = false;

        };


        /// Metadata for one concrete TransportSet.
        template<class... TTransports>
        struct TransportSetTraits<
            TransportSet<TTransports...>
        > final {

            /// Indicates that the inspected Type is a common TransportSet.
            static constexpr bool IsValid = true;

        };


        /// Default deployment-binding metadata for unrelated Types.
        ///
        /// @tparam TDeclaration Type being inspected.
        template<class TDeclaration>
        struct DeploymentDeclarationTraits final {

            /// Indicates whether the inspected Type is a common deployment declaration.
            static constexpr bool IsValid = false;

        };


        /// Metadata for one inbound deployment declaration.
        template<class TPrimitive, class TTransport>
        struct DeploymentDeclarationTraits<
            Inbound<TPrimitive, TTransport>
        > final {

            /// Indicates that this declaration uses common deployment vocabulary.
            static constexpr bool IsValid = true;

            /// Indicates that the declaration expands to inbound bindings.
            static constexpr bool HasInbound = true;

            /// Indicates that the declaration expands to no outbound bindings.
            static constexpr bool HasOutbound = false;

            /// Primitive Type represented by the declaration.
            using Primitive = TPrimitive;

            /// Transport selector represented by the declaration.
            using Transport = TTransport;

        };


        /// Metadata for one outbound deployment declaration.
        template<class TPrimitive, class TTransport>
        struct DeploymentDeclarationTraits<
            Outbound<TPrimitive, TTransport>
        > final {

            /// Indicates that this declaration uses common deployment vocabulary.
            static constexpr bool IsValid = true;

            /// Indicates that the declaration expands to no inbound bindings.
            static constexpr bool HasInbound = false;

            /// Indicates that the declaration expands to outbound bindings.
            static constexpr bool HasOutbound = true;

            /// Primitive Type represented by the declaration.
            using Primitive = TPrimitive;

            /// Transport selector represented by the declaration.
            using Transport = TTransport;

        };


        /// Metadata for bidirectional deployment sugar.
        template<class TPrimitive, class TTransport>
        struct DeploymentDeclarationTraits<
            Bidirectional<TPrimitive, TTransport>
        > final {

            /// Indicates that this declaration uses common deployment vocabulary.
            static constexpr bool IsValid = true;

            /// Indicates that the declaration expands to inbound bindings.
            static constexpr bool HasInbound = true;

            /// Indicates that the declaration expands to outbound bindings.
            static constexpr bool HasOutbound = true;

            /// Primitive Type represented by the declaration.
            using Primitive = TPrimitive;

            /// Transport selector represented by the declaration.
            using Transport = TTransport;

        };


        /// Expands one direction over one concrete transport list.
        ///
        /// @tparam TPrimitive Primitive Type being exposed.
        /// @tparam TTransports Concrete transport TypeList.
        /// @tparam TDirection Direction assigned to every emitted binding.
        template<
            class TPrimitive,
            class TTransports,
            DeploymentDirection TDirection
        >
        struct BindAllTransports;


        /// Emits one explicit binding for every configured concrete transport.
        template<
            class TPrimitive,
            class... TTransports,
            DeploymentDirection TDirection
        >
        struct BindAllTransports<
            TPrimitive,
            TypeList<TTransports...>,
            TDirection
        > final {

            /// Explicit bindings in configured transport order.
            using Type = TypeList<
                TransportBinding<
                    TPrimitive,
                    TTransports,
                    TDirection
                >...
            >;

        };


        /// Expands one common deployment declaration to explicit bindings.
        ///
        /// @tparam TDeclaration Common deployment declaration.
        /// @tparam TTransportSet Statically configured transport set.
        template<class TDeclaration, class TTransportSet>
        struct ExpandDeploymentDeclaration {

            static_assert(
                DeploymentDeclarationTraits<TDeclaration>::IsValid,
                "Family plan Bindings must contain Inbound, Outbound, or Bidirectional declarations"
            );

        };


        /// Expands one concrete inbound declaration.
        template<class TPrimitive, class TTransport, class... TConfiguredTransports>
        struct ExpandDeploymentDeclaration<
            Inbound<TPrimitive, TTransport>,
            TransportSet<TConfiguredTransports...>
        > final {

            static_assert(
                std::is_same_v<TTransport, AllTransports> ||
                TransportSet<TConfiguredTransports...>::template Contains<TTransport>,
                "Inbound references a transport that is not configured by the Topology"
            );

            /// Explicit inbound bindings represented by the declaration.
            using Type = std::conditional_t<
                std::is_same_v<TTransport, AllTransports>,
                typename BindAllTransports<
                    TPrimitive,
                    TypeList<TConfiguredTransports...>,
                    DeploymentDirection::Inbound
                >::Type,
                TypeList<
                    TransportBinding<
                        TPrimitive,
                        TTransport,
                        DeploymentDirection::Inbound
                    >
                >
            >;

        };


        /// Expands one concrete outbound declaration.
        template<class TPrimitive, class TTransport, class... TConfiguredTransports>
        struct ExpandDeploymentDeclaration<
            Outbound<TPrimitive, TTransport>,
            TransportSet<TConfiguredTransports...>
        > final {

            static_assert(
                std::is_same_v<TTransport, AllTransports> ||
                TransportSet<TConfiguredTransports...>::template Contains<TTransport>,
                "Outbound references a transport that is not configured by the Topology"
            );

            /// Explicit outbound bindings represented by the declaration.
            using Type = std::conditional_t<
                std::is_same_v<TTransport, AllTransports>,
                typename BindAllTransports<
                    TPrimitive,
                    TypeList<TConfiguredTransports...>,
                    DeploymentDirection::Outbound
                >::Type,
                TypeList<
                    TransportBinding<
                        TPrimitive,
                        TTransport,
                        DeploymentDirection::Outbound
                    >
                >
            >;

        };


        /// Expands bidirectional sugar to explicit inbound and outbound bindings.
        template<class TPrimitive, class TTransport, class... TConfiguredTransports>
        struct ExpandDeploymentDeclaration<
            Bidirectional<TPrimitive, TTransport>,
            TransportSet<TConfiguredTransports...>
        > final {

        private:

            /// Expanded inbound half.
            using InboundBindings = typename ExpandDeploymentDeclaration<
                Inbound<TPrimitive, TTransport>,
                TransportSet<TConfiguredTransports...>
            >::Type;

            /// Expanded outbound half.
            using OutboundBindings = typename ExpandDeploymentDeclaration<
                Outbound<TPrimitive, TTransport>,
                TransportSet<TConfiguredTransports...>
            >::Type;


        public:

            /// Explicit normalized bindings represented by the bidirectional declaration.
            using Type = typename ConcatTypeLists<
                InboundBindings,
                OutboundBindings
            >::Type;

        };


        /// Expands a list of common deployment declarations.
        ///
        /// @tparam TDeclarations Common deployment declaration TypeList.
        /// @tparam TTransportSet Statically configured transport set.
        template<class TDeclarations, class TTransportSet>
        struct ExpandDeploymentDeclarations;


        /// Completes binding expansion for an empty declaration list.
        template<class TTransportSet>
        struct ExpandDeploymentDeclarations<
            TypeList<>,
            TTransportSet
        > final {

            /// Empty explicit binding list.
            using Type = TypeList<>;

        };


        /// Expands one declaration and recursively expands the remainder.
        template<
            class TFirst,
            class... TRest,
            class TTransportSet
        >
        struct ExpandDeploymentDeclarations<
            TypeList<TFirst, TRest...>,
            TTransportSet
        > final {

        private:

            /// Bindings emitted by the current declaration.
            using Current = typename ExpandDeploymentDeclaration<
                TFirst,
                TTransportSet
            >::Type;

            /// Bindings emitted by remaining declarations.
            using Remaining = typename ExpandDeploymentDeclarations<
                TypeList<TRest...>,
                TTransportSet
            >::Type;


        public:

            /// Complete explicit binding list in deterministic declaration order.
            using Type = typename ConcatTypeLists<
                Current,
                Remaining
            >::Type;

        };

    } // ESPressio::Primitives::Detail

} // ESPressio::Primitives
