#pragma once

#include <type_traits>

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


    /// Canonical family-planner output consumed by common Topology aggregation.
    ///
    /// @tparam TFamily Primitive family represented by this plan.
    /// @tparam TRuntimeProvider Exactly one canonical family runtime provider Type.
    /// @tparam TPrimitiveTypes Primitive semantic Types deployed for the family.
    /// @tparam TResources Decomposed bounded family resource plan.
    template<
        class TFamily,
        class TRuntimeProvider,
        class TPrimitiveTypes,
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
        ///
        /// @tparam TPlan Candidate FamilyPlan Type exposing the canonical nested metadata.
        template<class TPlan>
        struct FamilyPlanTraits<
            TPlan,
            std::void_t<
                typename TPlan::FamilyPlanTag,
                typename TPlan::Family,
                typename TPlan::RuntimeProvider,
                typename TPlan::PrimitiveTypes,
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
        ///
        /// @tparam TFamily Expected Primitive family.
        /// @tparam TPrimitives Primitive Types represented by the list.
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
        ///
        /// @tparam TRequirements ResourceRequirement declarations represented by the plan.
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
        ///
        /// @tparam TTypes Types represented by the list.
        template<class... TTypes>
        struct TypeListTraits<
            TypeList<TTypes...>
        > final {

            /// Indicates that the Type is a common TypeList.
            static constexpr bool IsValid = true;

        };


        /// Validates canonical Planner output before Topology aggregation.
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


    /// Predicate identifying one canonical Planner implementation for a specific planning input.
    ///
    /// A family Planner is associated directly through Family::Planner. Its Plan alias
    /// receives the complete declaration set for that family and must emit canonical
    /// Transport-independent FamilyPlan output.
    ///
    /// @tparam TPlanner Candidate Planner Type.
    /// @tparam TFamily Primitive family being planned.
    /// @tparam TDeclarations Complete family-owned deployment declaration TypeList.
    template<
        class TPlanner,
        class TFamily,
        class TDeclarations
    >
    concept FamilyPlannerFor =
        PrimitiveFamilyType<TFamily> &&
        Detail::TypeListTraits<TDeclarations>::IsValid &&
        requires {
            typename TPlanner::template Plan<
                TFamily,
                TDeclarations
            >;
        } &&
        Detail::FamilyPlanTraits<
            typename TPlanner::template Plan<
                TFamily,
                TDeclarations
            >
        >::IsValid;

} // ESPressio::Primitives
