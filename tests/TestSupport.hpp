#pragma once

#include <cstdint>

#include <ESPressio_Primitives.hpp>

namespace ESPressio::Primitives::Tests::Support {

    namespace Framework = ESPressio::System::CompositionFramework;


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


    /// First zero-field schema-bearing Primitive Type in TestFamily.
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

        /// Canonical zero-field schema.
        using Fields = System::FieldSet<>;

        /// Primitive family classification.
        using Family = TestFamily;

    };


    /// Second zero-field schema-bearing Primitive Type in TestFamily.
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

        /// Canonical zero-field schema.
        using Fields = System::FieldSet<>;

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


    /// Dummy Planner exercising common family planning without depending on a real family repository.
    struct TestPlanner final {

        /// Validates the complete declaration set and exposes canonical plan output.
        ///
        /// @tparam TFamily Planned Primitive family.
        /// @tparam TDeclarations Family-owned declarations grouped by Topology.
        template<class TFamily, class TDeclarations>
        struct PlanBuilder final {

            static_assert(
                TDeclarations::Count == 2U,
                "Test Planner must receive the complete two-declaration family set"
            );

            static_assert(
                TDeclarations::template Contains<DeploymentOne> &&
                TDeclarations::template Contains<DeploymentTwo>,
                "Test Planner must receive both family-owned declarations"
            );

            /// Canonical test family plan.
            using Type = FamilyPlan<
                TFamily,
                TestRuntimeProvider,
                TypeList<
                    PrimitiveA,
                    PrimitiveB
                >,
                ResourcePlan<
                    ResourceRequirement<
                        QueueSlots,
                        8U
                    >
                >
            >;

        };


        /// Produces one canonical family plan from the complete family declaration set.
        ///
        /// @tparam TFamily Planned Primitive family.
        /// @tparam TDeclarations Family-owned declarations grouped by Topology.
        template<class TFamily, class TDeclarations>
        using Plan = typename PlanBuilder<
            TFamily,
            TDeclarations
        >::Type;

    };

} // ESPressio::Primitives::Tests::Support
