#include <cstdint>

#include <ESPressio_Primitives.hpp>

namespace Demo {

    namespace Framework = ESPressio::System::CompositionFramework;
    namespace Primitives = ESPressio::Primitives;


    /// Result of executing the compile-time Primitive-topology demonstration.
    enum class DemoResult : std::uint8_t {
        Succeeded = 0U,
        FamilyIdentityInvalid = 1U,
        UnexpectedBindingCount = 2U,
        UnexpectedProviderCount = 3U
    };


    /// First dummy transport configured by the application.
    struct TransportA final {};

    /// Second dummy transport configured by the application.
    struct TransportB final {};


    struct Planner;


    /// Demonstration Primitive family.
    struct Family final {

        /// Stable family identity under System Type Authority 1.
        static constexpr Primitives::PrimitiveFamilyIdentifier Identifier{
            ESPressio::System::TypeAuthorityIdentifier{
                ESPressio::System::TypeAuthorityIdentifier::Storage{
                    0x00U,
                    0x00U,
                    0x01U
                }
            },
            0x01U
        };

        /// Canonical family Planner.
        using Planner = Demo::Planner;

    };


    /// First semantic Primitive Type.
    struct StartSignal final {

        /// Stable universal Type identity.
        static constexpr ESPressio::System::TypeIdentifier Identifier{
            ESPressio::System::TypeIdentifier::Storage{
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
        using Family = Demo::Family;

    };


    /// Second semantic Primitive Type.
    struct StopSignal final {

        /// Stable universal Type identity.
        static constexpr ESPressio::System::TypeIdentifier Identifier{
            ESPressio::System::TypeIdentifier::Storage{
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
        using Family = Demo::Family;

    };


    /// Family-owned application deployment declaration.
    struct Deployment final {

        /// Family that interprets this declaration.
        using Family = Demo::Family;

    };


    /// Family-owned bounded queue-slot resource dimension.
    struct QueueSlots final {};


    /// Canonical runtime provider for the demonstration family.
    class RuntimeProvider final : public Framework::Provider<
        Primitives::Composition::Domain,
        Framework::Offers<
            Framework::Offer<
                Primitives::Composition::FamilyRuntime<Family>
            >
        >
    > {};


    /// Family-owned compile-time Planner.
    struct Planner final {

        /// Converts the complete family declaration set to common normalized vocabulary.
        template<class TFamily, class TDeclarations, class TTransportSet>
        using Plan = Primitives::FamilyPlan<
            TFamily,
            RuntimeProvider,
            Primitives::TypeList<
                StartSignal,
                StopSignal
            >,
            Primitives::TypeList<
                Primitives::Bidirectional<
                    StartSignal,
                    Primitives::AllTransports
                >,
                Primitives::Outbound<
                    StopSignal,
                    TransportA
                >
            >,
            Primitives::ResourcePlan<
                Primitives::ResourceRequirement<
                    QueueSlots,
                    8U
                >
            >
        >;

        /// Accepts the demonstration's requested transport bindings.
        template<
            class TPrimitive,
            class TTransport,
            Primitives::DeploymentDirection TDirection
        >
        static constexpr bool BindingEligible = true;

    };


    /// Application Primitive topology.
    using ApplicationTopology = Primitives::Topology<
        Primitives::TransportSet<
            TransportA,
            TransportB
        >,
        Deployment
    >;


    /// Normal Primitive Composition participates directly in System Architecture.
    using ApplicationArchitecture = Framework::Architecture<
        ApplicationTopology::Composition
    >;


    static_assert(
        ApplicationArchitecture::IsValid,
        "Primitive Composition must participate in the normal System Architecture"
    );


    /// Executes the tiny runtime-observable part of the demonstration.
    DemoResult Run() noexcept {
        if (!Family::Identifier.IsValid()) {
            return DemoResult::FamilyIdentityInvalid;
        }

        if (ApplicationTopology::Bindings::Count != 5U) {
            return DemoResult::UnexpectedBindingCount;
        }

        if (ApplicationTopology::Composition::ProviderCount != 1U) {
            return DemoResult::UnexpectedProviderCount;
        }

        return DemoResult::Succeeded;
    }

} // Demo

/// Runs the demonstration from the ESP-IDF application entry point.
extern "C" void app_main() {
    static_cast<void>(Demo::Run());
}
