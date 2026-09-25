#include <ESPressio_Primitives.hpp>

namespace Test {

    namespace Framework = ESPressio::System::CompositionFramework;
    namespace Primitives = ESPressio::Primitives;

    struct Planner;

    struct Family final {

        static constexpr Primitives::PrimitiveFamilyIdentifier Identifier{
            ESPressio::System::TypeAuthorityIdentifier{
                ESPressio::System::TypeAuthorityIdentifier::Storage{
                    0x00U, 0x00U, 0x09U
                }
            },
            0x01U
        };

        using Planner = Test::Planner;

    };


    struct Primitive final {

        static constexpr ESPressio::System::TypeIdentifier Identifier{
            ESPressio::System::TypeIdentifier::Storage{
                0x00U, 0x00U, 0x09U, 0x00U,
                0x00U, 0x00U, 0x00U, 0x01U
            }
        };

        using Family = Test::Family;

    };


    struct Deployment final {

        using Family = Test::Family;

    };


    /// Unrelated capability proving that the provider is structurally valid but not a family runtime provider.
    struct OtherCapability final : Framework::ExclusiveCapability<
        Primitives::Composition::Domain
    > {};


    /// Structurally valid Primitive-domain provider which intentionally omits FamilyRuntime<Family>.
    class InvalidProvider final : public Framework::Provider<
        Primitives::Composition::Domain,
        Framework::Offers<
            Framework::Offer<
                OtherCapability
            >
        >
    > {};


    struct Planner final {

        template<class TFamily, class TDeclarations>
        using Plan = Primitives::FamilyPlan<
            TFamily,
            InvalidProvider,
            Primitives::TypeList<Primitive>,
            Primitives::ResourcePlan<>
        >;

    };


    using InvalidTopology = Primitives::Topology<
        Deployment
    >;

    static_assert(InvalidTopology::PrimitiveTypes::Count > 0U);

} // Test

int main() {
    return 0;
}
