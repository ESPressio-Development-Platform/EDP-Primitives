#include <ESPressio_Primitives.hpp>

namespace Test {

    namespace Framework = ESPressio::System::CompositionFramework;
    namespace Primitives = ESPressio::Primitives;

    struct Transport final {};
    struct Planner;

    struct Family final {
        static constexpr Primitives::PrimitiveFamilyIdentifier Identifier{
            ESPressio::System::TypeAuthorityIdentifier{
                ESPressio::System::TypeAuthorityIdentifier::Storage{
                    0x00U, 0x00U, 0x02U
                }
            },
            0x01U
        };
        using Planner = Test::Planner;
    };

    struct Primitive final {
        static constexpr ESPressio::System::TypeIdentifier Identifier{
            ESPressio::System::TypeIdentifier::Storage{
                0x00U, 0x00U, 0x02U, 0x00U,
                0x00U, 0x00U, 0x00U, 0x01U
            }
        };
        using Family = Test::Family;
    };

    struct Deployment final {
        using Family = Test::Family;
    };

    class Provider final : public Framework::Provider<
        Primitives::Composition::Domain,
        Framework::Offers<
            Framework::Offer<
                Primitives::Composition::FamilyRuntime<Family>
            >
        >
    > {};

    struct Planner final {
        template<class TFamily, class TDeclarations, class TTransportSet>
        using Plan = Primitives::FamilyPlan<
            TFamily,
            Provider,
            Primitives::TypeList<Primitive>,
            Primitives::TypeList<
                Primitives::Inbound<Primitive, Transport>,
                Primitives::Bidirectional<Primitive, Transport>
            >,
            Primitives::ResourcePlan<>
        >;

        template<class TPrimitive, class TTransport, Primitives::DeploymentDirection TDirection>
        static constexpr bool BindingEligible = true;
    };

    using InvalidTopology = Primitives::Topology<
        Primitives::TransportSet<Transport>,
        Deployment
    >;

    static_assert(InvalidTopology::Bindings::Count > 0U);

} // Test

int main() {
    return 0;
}
