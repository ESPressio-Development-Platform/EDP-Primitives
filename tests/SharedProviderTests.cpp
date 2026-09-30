#include <ESPressio_Primitives.hpp>

namespace ESPressio::Primitives::Tests::SharedProvider {

    namespace Framework = ESPressio::System::CompositionFramework;


    struct PlannerA;
    struct PlannerB;


    struct FamilyA final {

        static constexpr PrimitiveFamilyIdentifier Identifier{
            System::TypeAuthorityIdentifier{
                System::TypeAuthorityIdentifier::Storage{
                    0x00U,
                    0x00U,
                    0x05U
                }
            },
            0x01U
        };

        using Planner = PlannerA;

    };


    struct FamilyB final {

        static constexpr PrimitiveFamilyIdentifier Identifier{
            System::TypeAuthorityIdentifier{
                System::TypeAuthorityIdentifier::Storage{
                    0x00U,
                    0x00U,
                    0x05U
                }
            },
            0x02U
        };

        using Planner = PlannerB;

    };


    struct PrimitiveA final {

        static constexpr System::TypeIdentifier Identifier{
            System::TypeIdentifier::Storage{
                0x00U, 0x00U, 0x05U, 0x00U,
                0x00U, 0x00U, 0x00U, 0x01U
            }
        };

        using Fields = System::FieldSet<>;
        using Family = FamilyA;

    };


    struct PrimitiveB final {

        static constexpr System::TypeIdentifier Identifier{
            System::TypeIdentifier::Storage{
                0x00U, 0x00U, 0x05U, 0x00U,
                0x00U, 0x00U, 0x00U, 0x02U
            }
        };

        using Fields = System::FieldSet<>;
        using Family = FamilyB;

    };


    struct DeploymentA final {

        using Family = FamilyA;

    };


    struct DeploymentB final {

        using Family = FamilyB;

    };


    class SharedRuntimeProvider final : public Framework::Provider<
        Composition::Domain,
        Framework::Offers<
            Framework::Offer<
                Composition::FamilyRuntime<FamilyA>
            >,
            Framework::Offer<
                Composition::FamilyRuntime<FamilyB>
            >
        >
    > {};


    struct PlannerA final {

        template<class TFamily, class TDeclarations>
        using Plan = FamilyPlan<
            TFamily,
            SharedRuntimeProvider,
            TypeList<PrimitiveA>,
            ResourcePlan<>
        >;

    };


    struct PlannerB final {

        template<class TFamily, class TDeclarations>
        using Plan = FamilyPlan<
            TFamily,
            SharedRuntimeProvider,
            TypeList<PrimitiveB>,
            ResourcePlan<>
        >;

    };


    using SharedTopology = Topology<
        DeploymentA,
        DeploymentB
    >;


    static_assert(
        SharedTopology::Families::Count == 2U,
        "Both Primitive families must remain independently planned"
    );

    static_assert(
        SharedTopology::FamilyPlans::Count == 2U,
        "Both canonical family plans must remain visible"
    );

    static_assert(
        SharedTopology::Composition::ProviderCount == 1U,
        "One Provider Type satisfying both family runtime capabilities must appear once in Composition"
    );

    static_assert(
        SharedTopology::Composition::IsValid,
        "The de-duplicated shared runtime Provider must form a valid Primitive Composition"
    );


    using SharedArchitecture = Framework::Architecture<
        SharedTopology::Composition
    >;


    static_assert(
        SharedArchitecture::IsValid,
        "The shared-provider Primitive Composition must participate in System Architecture"
    );

} // ESPressio::Primitives::Tests::SharedProvider


int main() {
    return 0;
}
