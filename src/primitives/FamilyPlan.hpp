#pragma once

#include <type_traits>

#include "Deployment.hpp"
#include "PrimitiveComposition.hpp"
#include "ResourcePlan.hpp"

namespace ESPressio::Primitives {

    /// Family-owned deployment declaration accepted by Topology.
    ///
    /// A declaration's internal vocabulary belongs to its family library. EDP-Primitives
    /// requires only a public nested Family Type so declarations can be grouped before the
    /// family's canonical Planner interprets the complete set.
    ///
    /// @tparam TDeclaration Candidate family-owned deployment declaration.
    template<class TDeclaration>
    concept FamilyDeploymentDeclaration = requires {
        typename TDeclaration::Family;
    } && PrimitiveFamilyType<typename TDeclaration::Family>;


    /// Canonical family-planner output consumed by common Topology normalization.
    ///
    /// @tparam TFamily Primitive family represented by this plan.
    /// @tparam TRuntimeProvider Exactly one canonical family runtime provider Type.
    /// @tparam TPrimitiveTypes Primitive semantic Types deployed for the family.
    /// @tparam TBindings Common deployment declarations emitted by the family planner.
    /// @tparam TResources Decomposed bounded family resource plan.
    template<
        class TFamily,
        class TRuntimeProvider,
        class TPrimitiveTypes,
        class TBindings,
        class TResources
    >
    struct FamilyPlan final {

        static_assert(
            PrimitiveFamilyType<TFamily>,
            "FamilyPlan requires a valid Primitive family Type"
        );

        /// Marker identifying canonical family-planner output.
        using FamilyPlanTag = void;

        /// Primitive family represented by this plan.
        using Family = TFamily;

        /// Exactly one canonical family runtime provider Type.
        using RuntimeProvider = TRuntimeProvider;

        /// Primitive semantic Types deployed by the family.
        using PrimitiveTypes = TPrimitiveTypes;

        /// Common Inbound/Outbound/Bidirectional declarations emitted by the family planner.
        using Bindings = TBindings;

        /// Decomposed bounded resource requirements owned by the family runtime.
        using Resources = TResources;

    };


    namespace Detail {

        /// Default FamilyPlan metadata for unrelated Types.
        ///
        /// @tparam TPlan Type being inspected.
        template<class TPlan, class = void>
        struct FamilyPlanTraits final {

            /// Indicates whether the Type exposes canonical FamilyPlan metadata.
            static constexpr bool IsValid = false;

        };


        /// Extracts canonical FamilyPlan metadata.
        template<class TPlan>
        struct FamilyPlanTraits<
            TPlan,
            std::void_t<
                typename TPlan::FamilyPlanTag,
                typename TPlan::Family,
                typename TPlan::RuntimeProvider,
                typename TPlan::PrimitiveTypes,
                typename TPlan::Bindings,
                typename TPlan::Resources
            >
        > final {

            /// Indicates whether the represented family is valid.
            static constexpr bool IsValid =
                PrimitiveFamilyType<typename TPlan::Family>;

        };


        /// Indicates whether every Type in one list is a Primitive belonging to a specific family.
        ///
        /// @tparam TFamily Expected Primitive family.
        /// @tparam TPrimitiveList Primitive TypeList being validated.
        template<class TFamily, class TPrimitiveList>
        struct PrimitiveListForFamily;


        /// Validates one concrete Primitive Type pack.
        template<class TFamily, class... TPrimitives>
        struct PrimitiveListForFamily<
            TFamily,
            TypeList<TPrimitives...>
        > final {

            /// Indicates whether all Types are valid, unique Primitives of the requested family.
            static constexpr bool IsValid =
                (PrimitiveType<TPrimitives> && ...) &&
                (std::is_same_v<PrimitiveFamilyOf<TPrimitives>, TFamily> && ...) &&
                !HasDuplicateTypes<TypeList<TPrimitives...>>::Value;

        };


        /// Default ResourcePlan metadata for unrelated Types.
        ///
        /// @tparam TType Type being inspected.
        template<class TType>
        struct ResourcePlanTraits final {

            /// Indicates whether the Type is a ResourcePlan.
            static constexpr bool IsValid = false;

        };


        /// Metadata for a concrete ResourcePlan.
        template<class... TRequirements>
        struct ResourcePlanTraits<
            ResourcePlan<TRequirements...>
        > final {

            /// Indicates that the Type is a common ResourcePlan.
            static constexpr bool IsValid = true;

        };


        /// Default TypeList metadata for unrelated Types.
        ///
        /// @tparam TType Type being inspected.
        template<class TType>
        struct TypeListTraits final {

            /// Indicates whether the Type is a common TypeList.
            static constexpr bool IsValid = false;

        };


        /// Metadata for a concrete TypeList.
        template<class... TTypes>
        struct TypeListTraits<
            TypeList<TTypes...>
        > final {

            /// Indicates that the Type is a common TypeList.
            static constexpr bool IsValid = true;

        };


        /// Validates canonical planner output before normalization.
        ///
        /// @tparam TFamily Family whose Planner produced the output.
        /// @tparam TPlan Planner output Type.
        template<class TFamily, class TPlan>
        struct ValidateFamilyPlan final {

            static_assert(
                FamilyPlanTraits<TPlan>::IsValid,
                "Family::Planner must emit canonical Primitives::FamilyPlan output"
            );

            static_assert(
                std::is_same_v<typename TPlan::Family, TFamily>,
                "Family::Planner output Family must match the family being planned"
            );

            static_assert(
                TypeListTraits<typename TPlan::PrimitiveTypes>::IsValid,
                "FamilyPlan PrimitiveTypes must be Primitives::TypeList"
            );

            static_assert(
                PrimitiveListForFamily<
                    TFamily,
                    typename TPlan::PrimitiveTypes
                >::IsValid,
                "FamilyPlan PrimitiveTypes must contain unique valid Primitive Types from the planned family"
            );

            static_assert(
                TypeListTraits<typename TPlan::Bindings>::IsValid,
                "FamilyPlan Bindings must be Primitives::TypeList"
            );

            static_assert(
                ResourcePlanTraits<typename TPlan::Resources>::IsValid,
                "FamilyPlan Resources must be Primitives::ResourcePlan"
            );

            static_assert(
                Composition::FamilyRuntimeProvider<
                    typename TPlan::RuntimeProvider,
                    TFamily
                >,
                "FamilyPlan RuntimeProvider must provide Composition::FamilyRuntime for the planned family"
            );

            /// Validated canonical family plan.
            using Type = TPlan;

        };

    } // ESPressio::Primitives::Detail

} // ESPressio::Primitives
