#pragma once

#include <type_traits>

#include <ESPressio_System.hpp>

#include "PrimitiveType.hpp"

namespace ESPressio::Primitives::Composition {

    namespace Framework = ESPressio::System::CompositionFramework;


    /// Composition Domain containing Primitive family runtime providers.
    struct Domain final : Framework::Domain {};


    /// Exclusive runtime capability owned by one Primitive family.
    ///
    /// @tparam TFamily Primitive family whose runtime provider is represented.
    template<class TFamily>
    struct FamilyRuntime final : Framework::ExclusiveCapability<Domain> {

        static_assert(
            PrimitiveFamilyType<TFamily>,
            "FamilyRuntime requires a valid Primitive family Type"
        );

    };


    namespace Detail {

        /// Determines whether one Type exposes the public Provider metadata required for inspection.
        ///
        /// @tparam TProvider Candidate provider Type.
        template<class TProvider>
        concept ProviderDeclaration = requires {
            typename TProvider::ProviderDeclarationTag;
            typename TProvider::CompositionDomain;
            typename TProvider::CompositionOffers;
            typename TProvider::CompositionContract;
        };


        /// Determines whether one provider is the canonical runtime provider for one family.
        ///
        /// @tparam TProvider Candidate provider Type.
        /// @tparam TFamily Primitive family represented by the expected capability.
        template<class TProvider, class TFamily>
        consteval bool IsFamilyRuntimeProvider() {
            if constexpr (!ProviderDeclaration<TProvider>) return false;
            if constexpr (!PrimitiveFamilyType<TFamily>) return false;
            if constexpr (!std::is_same_v<typename TProvider::CompositionDomain, Domain>) return false;

            return TProvider::CompositionOffers::template Contains<
                FamilyRuntime<TFamily>
            >;
        }

    } // ESPressio::Primitives::Composition::Detail


    /// Predicate identifying a Primitive Domain provider for one exact family runtime capability.
    ///
    /// @tparam TProvider Candidate runtime provider Type.
    /// @tparam TFamily Primitive family whose runtime capability must be supplied.
    template<class TProvider, class TFamily>
    concept FamilyRuntimeProvider =
        Detail::IsFamilyRuntimeProvider<TProvider, TFamily>();

} // ESPressio::Primitives::Composition
