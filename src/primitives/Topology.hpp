#pragma once

#include <type_traits>

#include "FamilyPlan.hpp"

namespace ESPressio::Primitives {

    namespace Detail {

        /// Collects deployment declarations belonging to one family.
        ///
        /// @tparam TFamily Family whose declarations are requested.
        /// @tparam TDeclarations Complete family-deployment declaration list.
        template<class TFamily, class TDeclarations>
        struct FilterFamilyDeclarations;


        /// Completes family filtering for an empty declaration list.
        ///
        /// @tparam TFamily Family whose declarations are requested.
        template<class TFamily>
        struct FilterFamilyDeclarations<
            TFamily,
            TypeList<>
        > final {

            /// Empty matching declaration list.
            using Type = TypeList<>;

        };


        /// Filters one declaration and recursively filters the remainder.
        ///
        /// @tparam TFamily Family whose declarations are requested.
        /// @tparam TFirst First deployment declaration being inspected.
        /// @tparam TRest Remaining deployment declarations.
        template<
            class TFamily,
            class TFirst,
            class... TRest
        >
        struct FilterFamilyDeclarations<
            TFamily,
            TypeList<TFirst, TRest...>
        > final {

        private:

            /// Matching declarations from the remaining list.
            using Remaining = typename FilterFamilyDeclarations<
                TFamily,
                TypeList<TRest...>
            >::Type;


        public:

            /// Matching family declarations in original declaration order.
            using Type = std::conditional_t<
                std::is_same_v<typename TFirst::Family, TFamily>,
                typename ConcatTypeLists<
                    TypeList<TFirst>,
                    Remaining
                >::Type,
                Remaining
            >;

        };


        /// Extracts one deployment declaration's family.
        ///
        /// @tparam TDeclaration Family-owned deployment declaration.
        template<class TDeclaration>
        struct DeploymentFamily final {

            /// Primitive family declared by the deployment.
            using Type = typename TDeclaration::Family;

        };


        /// Builds the unique family list represented by deployment declarations.
        ///
        /// @tparam TDeclarations Family deployment declaration TypeList.
        template<class TDeclarations>
        struct DeploymentFamilies;


        /// Extracts unique families in first-declaration order.
        ///
        /// @tparam TDeclarations Family-owned deployment declarations.
        template<class... TDeclarations>
        struct DeploymentFamilies<
            TypeList<TDeclarations...>
        > final {

            /// Unique families represented by all declarations.
            using Type = typename UniqueTypeList<
                TypeList<
                    typename DeploymentFamily<TDeclarations>::Type...
                >
            >::Type;

        };


        /// Invokes one family's canonical Planner with the complete declaration set.
        ///
        /// The family Planner contract is:
        ///
        /// template<class TFamily, class TDeclarations>
        /// using Plan = Primitives::FamilyPlan<...>;
        ///
        /// @tparam TFamily Family being planned.
        /// @tparam TDeclarations Complete declarations for that family.
        template<
            class TFamily,
            class TDeclarations
        >
        struct InvokeFamilyPlanner final {

            /// Canonical Planner directly associated by the family tag.
            using Planner = typename TFamily::Planner;

            static_assert(
                FamilyPlannerFor<
                    Planner,
                    TFamily,
                    TDeclarations
                >,
                "Family::Planner must satisfy the public FamilyPlannerFor extension contract"
            );

            /// Raw canonical family plan emitted by the Planner.
            using RawPlan = typename Planner::template Plan<
                TFamily,
                TDeclarations
            >;

            /// Validated canonical family plan.
            using Type = typename ValidateFamilyPlan<
                TFamily,
                RawPlan
            >::Type;

        };


        /// Builds one FamilyPlan for each represented family.
        ///
        /// @tparam TFamilies Unique family TypeList.
        /// @tparam TDeclarations Complete deployment declaration list.
        template<
            class TFamilies,
            class TDeclarations
        >
        struct BuildFamilyPlans;


        /// Completes planning for an empty family list.
        ///
        /// @tparam TDeclarations Complete deployment declaration list.
        template<class TDeclarations>
        struct BuildFamilyPlans<
            TypeList<>,
            TDeclarations
        > final {

            /// Empty family-plan list.
            using Type = TypeList<>;

        };


        /// Plans one family and recursively plans the remainder.
        ///
        /// @tparam TFirstFamily Current Primitive family.
        /// @tparam TRestFamilies Remaining Primitive families.
        /// @tparam TDeclarations Complete deployment declaration list.
        template<
            class TFirstFamily,
            class... TRestFamilies,
            class TDeclarations
        >
        struct BuildFamilyPlans<
            TypeList<TFirstFamily, TRestFamilies...>,
            TDeclarations
        > final {

        private:

            /// Complete declarations belonging to the current family.
            using FamilyDeclarations = typename FilterFamilyDeclarations<
                TFirstFamily,
                TDeclarations
            >::Type;

            /// Current family's canonical validated plan.
            using CurrentPlan = typename InvokeFamilyPlanner<
                TFirstFamily,
                FamilyDeclarations
            >::Type;

            /// Plans emitted for the remaining families.
            using RemainingPlans = typename BuildFamilyPlans<
                TypeList<TRestFamilies...>,
                TDeclarations
            >::Type;


        public:

            /// Complete family-plan list in first-family declaration order.
            using Type = typename ConcatTypeLists<
                TypeList<CurrentPlan>,
                RemainingPlans
            >::Type;

        };


        /// Extracts the Primitive Type list from one family plan.
        ///
        /// @tparam TPlan Canonical validated FamilyPlan.
        template<class TPlan>
        struct PlanPrimitiveTypes final {

            /// Primitive Types represented by the plan.
            using Type = typename TPlan::PrimitiveTypes;

        };


        /// Concatenates one projected TypeList from every plan.
        ///
        /// @tparam TPlans FamilyPlan TypeList.
        /// @tparam TProjection Projection returning a nested Type.
        template<
            class TPlans,
            template<class> class TProjection
        >
        struct FlattenPlanLists;


        /// Completes flattening for an empty family-plan list.
        ///
        /// @tparam TProjection Projection returning a nested Type.
        template<template<class> class TProjection>
        struct FlattenPlanLists<
            TypeList<>,
            TProjection
        > final {

            /// Empty flattened list.
            using Type = TypeList<>;

        };


        /// Concatenates the current projected list with recursively flattened remainder.
        ///
        /// @tparam TFirst First family plan.
        /// @tparam TRest Remaining family plans.
        /// @tparam TProjection Projection returning a nested Type.
        template<
            class TFirst,
            class... TRest,
            template<class> class TProjection
        >
        struct FlattenPlanLists<
            TypeList<TFirst, TRest...>,
            TProjection
        > final {

            /// Complete flattened TypeList.
            using Type = typename ConcatTypeLists<
                typename TProjection<TFirst>::Type,
                typename FlattenPlanLists<
                    TypeList<TRest...>,
                    TProjection
                >::Type
            >::Type;

        };


        /// Indicates whether one Primitive identifier differs from every identifier in a remaining list.
        ///
        /// @tparam TPrimitive Primitive Type whose universal identity is being compared.
        /// @tparam TOtherPrimitives Other deployed Primitive Types.
        template<class TPrimitive, class... TOtherPrimitives>
        inline constexpr bool PrimitiveIdentifierUniqueAgainstV =
            (
                (
                    System::TypeIdentifierOf<TPrimitive> !=
                    System::TypeIdentifierOf<TOtherPrimitives>
                ) &&
                ... &&
                true
            );


        /// Validates topology-wide universal Primitive identity uniqueness.
        ///
        /// @tparam TPrimitiveTypes Deployed Primitive TypeList.
        template<class TPrimitiveTypes>
        struct UniquePrimitiveIdentifiers;


        /// Empty Primitive lists contain no identity collisions.
        template<>
        struct UniquePrimitiveIdentifiers<TypeList<>> : std::true_type {};


        /// Validates the first Primitive identity against the remainder and continues recursively.
        ///
        /// @tparam TFirstPrimitive First deployed Primitive Type.
        /// @tparam TRestPrimitives Remaining deployed Primitive Types.
        template<class TFirstPrimitive, class... TRestPrimitives>
        struct UniquePrimitiveIdentifiers<
            TypeList<TFirstPrimitive, TRestPrimitives...>
        > : std::bool_constant<
            PrimitiveIdentifierUniqueAgainstV<
                TFirstPrimitive,
                TRestPrimitives...
            > &&
            UniquePrimitiveIdentifiers<
                TypeList<TRestPrimitives...>
            >::value
        > {};


        /// Converts one runtime-provider TypeList to the Primitive System Composition.
        ///
        /// @tparam TProviders Unique canonical runtime provider Types.
        template<class TProviders>
        struct MakeCompositionFromProviders;


        /// Builds the System Composition from one concrete provider Type pack.
        ///
        /// @tparam TProviders Canonical runtime provider Types.
        template<class... TProviders>
        struct MakeCompositionFromProviders<
            TypeList<TProviders...>
        > final {

            /// Primitive Domain Composition consumable by System Architecture.
            using Type = System::CompositionFramework::Composition<
                Composition::Domain,
                TProviders...
            >;

        };


        /// Converts family plans to a Primitive Composition.
        ///
        /// A provider Type may legitimately satisfy more than one family runtime capability.
        /// The Composition therefore contains each runtime provider Type once, while each
        /// FamilyPlan is still independently validated against its exact FamilyRuntime capability.
        ///
        /// @tparam TPlans Canonical validated family plans.
        template<class TPlans>
        struct MakePrimitiveComposition;


        /// Builds the System Composition from unique canonical family runtime providers.
        ///
        /// @tparam TPlans Canonical validated family plans.
        template<class... TPlans>
        struct MakePrimitiveComposition<
            TypeList<TPlans...>
        > final {

        private:

            /// Runtime provider Types in deterministic family order.
            using ProviderTypes = TypeList<
                typename TPlans::RuntimeProvider...
            >;

            /// Runtime provider Types with duplicate Types removed after first occurrence.
            using UniqueProviderTypes = typename UniqueTypeList<
                ProviderTypes
            >::Type;


        public:

            /// Primitive Domain Composition consumable by System Architecture.
            using Type = typename MakeCompositionFromProviders<
                UniqueProviderTypes
            >::Type;

        };


        /// Builds the family-neutral ResourcePlanSet retained by Topology output.
        ///
        /// @tparam TPlans Canonical validated family plans.
        template<class TPlans>
        struct MakeResourcePlanSet;


        /// Retains one independently decomposed ResourcePlan per family.
        ///
        /// @tparam TPlans Canonical validated family plans.
        template<class... TPlans>
        struct MakeResourcePlanSet<
            TypeList<TPlans...>
        > final {

            /// Family-neutral resource aggregation without implicit cross-family totals.
            using Type = ResourcePlanSet<
                FamilyResources<
                    typename TPlans::Family,
                    typename TPlans::Resources
                >...
            >;

        };

    } // ESPressio::Primitives::Detail


    /// Compile-time Primitive deployment topology.
    ///
    /// Arguments are family-owned deployment declarations. No runtime object, registry,
    /// allocation, transport configuration, wildcard or discovery mechanism is created.
    ///
    /// @tparam TDeployments Family-owned deployment declarations.
    template<class... TDeployments>
    struct Topology final {

        static_assert(
            (FamilyDeploymentDeclaration<TDeployments> && ...),
            "Topology entries must be family-owned deployment declarations"
        );

        /// Family-owned declarations in application declaration order.
        using Deployments = TypeList<TDeployments...>;

        /// Unique represented families in first-declaration order.
        using Families = typename Detail::DeploymentFamilies<
            Deployments
        >::Type;

        /// Canonical validated plans emitted by each family's directly associated Planner.
        using FamilyPlans = typename Detail::BuildFamilyPlans<
            Families,
            Deployments
        >::Type;

        /// Every deployed Primitive Type across all families.
        using PrimitiveTypes = typename Detail::FlattenPlanLists<
            FamilyPlans,
            Detail::PlanPrimitiveTypes
        >::Type;

        static_assert(
            !Detail::HasDuplicateTypes<PrimitiveTypes>::Value,
            "Topology must not deploy the same Primitive Type more than once"
        );

        static_assert(
            Detail::UniquePrimitiveIdentifiers<PrimitiveTypes>::value,
            "Topology must not deploy distinct Primitive Types with the same System::TypeIdentifier"
        );

        /// Decomposed resource plans retained separately for every family.
        using Resources = typename Detail::MakeResourcePlanSet<
            FamilyPlans
        >::Type;

        /// Primitive Domain Composition built from exactly one runtime provider per family.
        using Composition = typename Detail::MakePrimitiveComposition<
            FamilyPlans
        >::Type;

        static_assert(
            Composition::IsValid,
            "Primitive family runtime providers must form a valid System Composition"
        );

    };

} // ESPressio::Primitives
