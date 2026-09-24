#include <type_traits>

#include <ESPressio_Primitives.hpp>

#include "TestSupport.hpp"

namespace ESPressio::Primitives::Tests {

    namespace Framework = ESPressio::System::CompositionFramework;


    static_assert(
        sizeof(PrimitiveFamilyIdentifier) == 4U,
        "PrimitiveFamilyIdentifier must occupy exactly four bytes"
    );

    static_assert(
        PrimitiveFamilyType<Support::TestFamily>,
        "TestFamily must satisfy the Primitive family contract"
    );

    static_assert(
        PrimitiveType<Support::PrimitiveA> &&
        PrimitiveType<Support::PrimitiveB>,
        "Test semantic Types must satisfy PrimitiveType"
    );

    static_assert(
        std::is_same_v<
            PrimitiveFamilyOf<Support::PrimitiveA>,
            Support::TestFamily
        >,
        "PrimitiveFamilyOf must return the semantic Primitive's direct family"
    );

    static_assert(
        PrimitiveFamilyIdentifierOf<Support::PrimitiveA> ==
        Support::TestFamily::Identifier,
        "PrimitiveFamilyIdentifierOf must return the family's stable identity"
    );


    /// Topology containing two family-owned declarations and two configured transports.
    using TestTopology = Topology<
        TransportSet<
            Support::TransportA,
            Support::TransportB
        >,
        Support::DeploymentOne,
        Support::DeploymentTwo
    >;


    static_assert(
        TestTopology::Families::Count == 1U,
        "Two declarations from one family must normalize to one family plan"
    );

    static_assert(
        TestTopology::FamilyPlans::Count == 1U,
        "Exactly one canonical Planner invocation is expected per represented family"
    );

    static_assert(
        TestTopology::NormalizedFamilyPlans::Count == 1U,
        "Exactly one normalized family plan is expected"
    );

    static_assert(
        TestTopology::PrimitiveTypes::Count == 2U,
        "Both deployed Primitive Types must appear exactly once"
    );

    static_assert(
        TestTopology::Bindings::Count == 5U,
        "Bidirectional AllTransports over two transports plus one outbound binding must normalize to five bindings"
    );

    static_assert(
        TestTopology::Bindings::template Contains<
            TransportBinding<
                Support::PrimitiveA,
                Support::TransportA,
                DeploymentDirection::Inbound
            >
        >,
        "PrimitiveA must expose inbound TransportA"
    );

    static_assert(
        TestTopology::Bindings::template Contains<
            TransportBinding<
                Support::PrimitiveA,
                Support::TransportB,
                DeploymentDirection::Outbound
            >
        >,
        "PrimitiveA must expose outbound TransportB"
    );

    static_assert(
        TestTopology::Bindings::template Contains<
            TransportBinding<
                Support::PrimitiveB,
                Support::TransportA,
                DeploymentDirection::Outbound
            >
        >,
        "PrimitiveB must expose the explicitly requested outbound transport"
    );

    static_assert(
        TestTopology::Resources::Count == 1U,
        "Resource aggregation must retain one decomposed plan per family"
    );

    static_assert(
        TestTopology::Composition::ProviderCount == 1U,
        "Primitive Composition must contain exactly one runtime provider per represented family"
    );

    static_assert(
        TestTopology::Composition::IsValid,
        "Generated Primitive Domain Composition must satisfy the existing System Composition model"
    );


    /// Complete application Architecture proving Primitive Composition is directly consumable.
    using TestArchitecture = Framework::Architecture<
        TestTopology::Composition
    >;


    static_assert(
        TestArchitecture::IsValid,
        "Primitive Composition must participate directly in System Architecture"
    );


    /// Executes value-level validation of the packed Primitive family identifier.
    int RunPrimitivesTests() noexcept {
        const PrimitiveFamilyIdentifier invalid;
        if (invalid.IsValid()) return 1;

        const auto identifier = Support::TestFamily::Identifier;
        if (!identifier.IsValid()) return 2;
        if (identifier.Value() != 0x00000101U) return 3;
        if (identifier.Authority().Value() != 0x000001U) return 4;
        if (identifier.LocalValue() != 0x01U) return 5;

        const PrimitiveFamilyIdentifier later(
            System::TypeAuthorityIdentifier{
                System::TypeAuthorityIdentifier::Storage{
                    0x00U,
                    0x00U,
                    0x01U
                }
            },
            0x02U
        );

        if (!(identifier < later)) return 6;
        if (later < identifier) return 7;

        return 0;
    }

} // ESPressio::Primitives::Tests


/// Executes host-side Stage 1 Primitive foundation tests.
int main() {
    return ESPressio::Primitives::Tests::RunPrimitivesTests();
}
