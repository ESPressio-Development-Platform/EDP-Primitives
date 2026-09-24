#pragma once

#include <cstdint>

#include <ESPressio_Primitives.hpp>

namespace ESPressio::Primitives::Tests::Support {

    namespace Framework = ESPressio::System::CompositionFramework;


    /// First deterministic dummy transport used by topology tests.
    struct TransportA final {};

    /// Second deterministic dummy transport used by topology tests.
    struct TransportB final {};


    struct TestPlanner;


    /// Dummy Primitive family used to validate common family/planner contracts.
    struct TestFamily final {

        /// Stable family identity under System Type Authority 1.
        static constexpr PrimitiveFamilyIdentifier Identifier{
            System::TypeAuthorityIdentifier{
                System::TypeAuthorityIdentifier::Storage{
                    0x00U,
                    0x00U,
                    0x01U
                }
            },
            0x01U
        };

        /// Canonical family planner.
        using Planner = TestPlanner;

    };


    /// First identified Primitive Type in TestFamily.
    struct PrimitiveA final {

        /// Stable universal Type identity.
        static constexpr System::TypeIdentifier Identifier{
            System::TypeIdentifier::Storage{
                0x00U,
                0x00U,
                0x01U,
                0x00U,
                0x00U,
                0x00U,
                0x00U,
                0x01U
            }
        };

        /// Primitive family classification.
        using Family = TestFamily;

    };


    /// Second identified Primitive Type in TestFamily.
    struct PrimitiveB final {

        /// Stable universal Type identity.
        static constexpr System::TypeIdentifier Identifier{
            System::TypeIdentifier::Storage{
                0x00U,
                0x00U,
                0x01U,
                0x00U,
                0x00U,
                0x00U,
                0x00U,
                0x02U
            }
        };

        /// Primitive family classification.
        using Family = TestFamily;

    };


    /// First family-owned deployment fragment used to verify declaration grouping.
    struct DeploymentOne final {

        /// Family that owns and interprets this declaration.
        using Family = TestFamily;

    };


    /// Second family-owned deployment fragment used to verify declaration grouping.
    struct DeploymentTwo final {

        /// Family that owns and interprets this declaration.
        using Family = TestFamily;

    };


    /// Dummy bounded family resource dimension.
    struct QueueSlots final {};


    /// Canonical runtime provider for TestFamily.
    class TestRuntimeProvider final : public Framework::Provider<
        Composition::Domain,
        Framework::Offers<
            Framework::Offer<
                Composition::FamilyRuntime<TestFamily>
            >
        >
    > {};


    /// Dummy Planner exercising common normalization without depending on a real family repository.
    struct TestPlanner final {

        /// Produces one canonical family plan from the complete family declaration set.
        ///
        /// @tparam TFamily Planned Primitive family.
        /// @tparam TDeclarations Family-owned declarations grouped by Topology.
        /// @tparam TTransportSet Statically configured transport set.
        template<
            class TFamily,
            class TDeclarations,
            class TTransportSet
        >
        using Plan = FamilyPlan<
            TFamily,
            TestRuntimeProvider,
            TypeList<
                PrimitiveA,
                PrimitiveB
            >,
            TypeList<
                Bidirectional<
                    PrimitiveA,
                    AllTransports
                >,
                Outbound<
                    PrimitiveB,
                    TransportA
                >
            >,
            ResourcePlan<
                ResourceRequirement<
                    QueueSlots,
                    8U
                >
            >
        >;

        /// Accepts all bindings used by this generic contract test.
        ///
        /// A concrete family Planner can implement serialisability and other family-owned
        /// compile-time eligibility rules here without introducing those dependencies into
        /// EDP-Primitives.
        template<
            class TPrimitive,
            class TTransport,
            DeploymentDirection TDirection
        >
        static constexpr bool BindingEligible = true;

    };

} // ESPressio::Primitives::Tests::Support
