#include <type_traits>

#include <ESPressio_Primitives.hpp>

#include "TestSupport.hpp"


static_assert(ESPressio::Primitives::ExecutionDomain::Scope<
    ESPressio::Primitives::ExecutionDomain::LocalOnly
>);
static_assert(ESPressio::Primitives::ExecutionDomain::Scope<
    const ESPressio::Primitives::ExecutionDomain::RemoteOnly&
>);
static_assert(ESPressio::Primitives::ExecutionDomain::Scope<
    volatile ESPressio::Primitives::ExecutionDomain::LocalAndRemote
>);
static_assert(!ESPressio::Primitives::ExecutionDomain::Scope<int>);

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


    /// Topology containing two family-owned declarations.
    using TestTopology = Topology<
        Support::DeploymentOne,
        Support::DeploymentTwo
    >;


    static_assert(
        TestTopology::Families::Count == 1U,
        "Two declarations from one family must produce one family plan"
    );

    static_assert(
        TestTopology::FamilyPlans::Count == 1U,
        "Exactly one canonical Planner invocation is expected per represented family"
    );

    static_assert(
        TestTopology::PrimitiveTypes::Count == 2U,
        "Both deployed Primitive Types must appear exactly once"
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


    /// Empty Primitive topology proving that optional Primitive deployment compiles away.
    using EmptyTopology = Topology<>;


    static_assert(
        EmptyTopology::Families::Count == 0U &&
        EmptyTopology::FamilyPlans::Count == 0U &&
        EmptyTopology::PrimitiveTypes::Count == 0U &&
        EmptyTopology::Resources::Count == 0U,
        "Empty Primitive topology must retain no family, Primitive, or resource declarations"
    );

    static_assert(
        EmptyTopology::Composition::ProviderCount == 0U &&
        EmptyTopology::Composition::IsValid,
        "Empty Primitive topology must produce a valid zero-provider Primitive Composition"
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


/// Executes host-side Primitive foundation tests.
int main() {
    return ESPressio::Primitives::Tests::RunPrimitivesTests();
}
