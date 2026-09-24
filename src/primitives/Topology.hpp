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
        template<class TFamily>
        struct FilterFamilyDeclarations<
            TFamily,
            TypeList<>
        > final {

            /// Empty matching declaration list.
            using Type = TypeList<>;

        };


        /// Filters one declaration and recursively filters the remainder.
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
        /// template<class TFamily, class TDeclarations, class TTransportSet>
        /// using Plan = Primitives::FamilyPlan<...>;
        ///
        /// and for every normalized binding:
        ///
        /// template<class TPrimitive, class TTransport, DeploymentDirection TDirection>
        /// static constexpr bool BindingEligible = ...;
        ///
        /// @tparam TFamily Family being planned.
        /// @tparam TDeclarations Complete declarations for that family.
        /// @tparam TTransportSet Statically configured transport set.
        template<
            class TFamily,
            class TDeclarations,
            class TTransportSet
        >
        struct InvokeFamilyPlanner final {

            /// Canonical Planner directly associated by the family tag.
            using Planner = typename TFamily::Planner;

            static_assert(
                FamilyPlannerFor<
                    Planner,
                    TFamily,
                    TDeclarations,
                    TTransportSet
                >,
                "Family::Planner must satisfy the public FamilyPlannerFor extension contract"
            );

            /// Raw canonical family plan emitted by the Planner.
            using RawPlan = typename Planner::template Plan<
                TFamily,
                TDeclarations,
                TTransportSet
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
        /// @tparam TTransportSet Configured transport set.
        template<
            class TFamilies,
            class TDeclarations,
            class TTransportSet
        >
        struct BuildFamilyPlans;


        /// Completes planning for an empty family list.
        template<
            class TDeclarations,
            class TTransportSet
        >
        struct BuildFamilyPlans<
            TypeList<>,
            TDeclarations,
            TTransportSet
        > final {

            /// Empty family-plan list.
            using Type = TypeList<>;

        };


        /// Plans one family and recursively plans the remainder.
        template<
            class TFirstFamily,
            class... TRestFamilies,
            class TDeclarations,
            class TTransportSet
        >
        struct BuildFamilyPlans<
            TypeList<TFirstFamily, TRestFamilies...>,
            TDeclarations,
            TTransportSet
        > final {

        private:

            /// Complete declarations belonging to the current family.
            using FamilyDeclarations = typename FilterFamilyDeclarations<
                TFirstFamily,
                TDeclarations
            >::Type;

            /// Current family's canonical plan.
            using CurrentPlan = typename InvokeFamilyPlanner<
                TFirstFamily,
                FamilyDeclarations,
                TTransportSet
            >::Type;

            /// Plans emitted for the remaining families.
            using RemainingPlans = typename BuildFamilyPlans<
                TypeList<TRestFamilies...>,
                TDeclarations,
                TTransportSet
            >::Type;


        public:

            /// Complete family-plan list in first-family declaration order.
            using Type = typename ConcatTypeLists<
                TypeList<CurrentPlan>,
                RemainingPlans
            >::Type;

        };


        /// Expands and validates every common binding emitted by one family Planner.
        ///
        /// @tparam TPlan Canonical FamilyPlan.
        /// @tparam TTransportSet Statically configured transport set.
        template<class TPlan, class TTransportSet>
        struct NormalizeFamilyPlan;


        /// Validates one explicit normalized binding through the family Planner.
        ///
        /// @tparam TPlanner Family Planner owning eligibility policy.
        /// @tparam TBinding Explicit normalized TransportBinding.
        template<class TPlanner, class TBinding>
        struct ValidateBinding;


        /// Validates one concrete normalized binding.
        template<
            class TPlanner,
            class TPrimitive,
            class TTransport,
            DeploymentDirection TDirection
        >
        struct ValidateBinding<
            TPlanner,
            TransportBinding<
                TPrimitive,
                TTransport,
                TDirection
            >
        > final {

            static_assert(
                requires {
                    TPlanner::template BindingEligible<
                        TPrimitive,
                        TTransport,
                        TDirection
                    >;
                },
                "Family::Planner must expose BindingEligible<Primitive, Transport, Direction>"
            );

            static_assert(
                TPlanner::template BindingEligible<
                    TPrimitive,
                    TTransport,
                    TDirection
                >,
                "Family::Planner rejected a requested Primitive transport binding"
            );

            /// Validated explicit binding.
            using Type = TransportBinding<
                TPrimitive,
                TTransport,
                TDirection
            >;

        };


        /// Validates every binding in one explicit binding list.
        ///
        /// @tparam TPlanner Family Planner owning eligibility policy.
        /// @tparam TBindings Explicit normalized bindings.
        template<class TPlanner, class TBindings>
        struct ValidateBindings;


        /// Validates one concrete explicit binding pack.
        template<class TPlanner, class... TBindings>
        struct ValidateBindings<
            TPlanner,
            TypeList<TBindings...>
        > final {

            /// Validated bindings in deterministic order.
            using Type = TypeList<
                typename ValidateBinding<
                    TPlanner,
                    TBindings
                >::Type...
            >;

        };


        /// Normalized family plan after common binding expansion and eligibility validation.
        ///
        /// @tparam TFamily Primitive family.
        /// @tparam TRuntimeProvider Canonical family runtime provider.
        /// @tparam TPrimitiveTypes Deployed Primitive Types.
        /// @tparam TBindings Explicit normalized transport bindings.
        /// @tparam TResources Decomposed bounded resource plan.
        template<
            class TFamily,
            class TRuntimeProvider,
            class TPrimitiveTypes,
            class TBindings,
            class TResources
        >
        struct NormalizedFamilyPlan final {

            /// Primitive family represented by this plan.
            using Family = TFamily;

            /// Canonical runtime provider for the family.
            using RuntimeProvider = TRuntimeProvider;

            /// Unique Primitive Types deployed by the family.
            using PrimitiveTypes = TPrimitiveTypes;

            /// Explicit normalized transport bindings.
            using Bindings = TBindings;

            /// Decomposed bounded resource plan.
            using Resources = TResources;

        };


        /// Indicates whether every explicit binding targets a Primitive deployed by the same family plan.
        ///
        /// @tparam TPrimitiveTypes Primitive Types deployed by the family.
        /// @tparam TBindings Explicit normalized transport bindings.
        template<class TPrimitiveTypes, class TBindings>
        struct BindingsTargetDeployedPrimitives;


        /// Validates one explicit binding pack against the family's deployed Primitive list.
        template<class TPrimitiveTypes, class... TBindings>
        struct BindingsTargetDeployedPrimitives<
            TPrimitiveTypes,
            TypeList<TBindings...>
        > final {

            /// Indicates whether every binding targets a deployed Primitive Type.
            static constexpr bool IsValid =
                (TPrimitiveTypes::template Contains<typename TBindings::Primitive> && ...);

        };


        /// Normalizes one canonical family plan.
        template<
            class TFamily,
            class TRuntimeProvider,
            class TPrimitiveTypes,
            class TBindingDeclarations,
            class TResources,
            class TTransportSet
        >
        struct NormalizeFamilyPlan<
            FamilyPlan<
                TFamily,
                TRuntimeProvider,
                TPrimitiveTypes,
                TBindingDeclarations,
                TResources
            >,
            TTransportSet
        > final {

        private:

            /// Canonical Planner associated by the represented family.
            using Planner = typename TFamily::Planner;

            /// Explicit bindings after AllTransports/Bidirectional expansion.
            using ExpandedBindings = typename ExpandDeploymentDeclarations<
                TBindingDeclarations,
                TTransportSet
            >::Type;

            static_assert(
                !HasDuplicateTypes<ExpandedBindings>::Value,
                "FamilyPlan contains duplicate transport bindings after normalization"
            );

            static_assert(
                BindingsTargetDeployedPrimitives<
                    TPrimitiveTypes,
                    ExpandedBindings
                >::IsValid,
                "FamilyPlan transport bindings must target Primitive Types deployed by that same family plan"
            );


        public:

            /// Fully normalized family plan.
            using Type = NormalizedFamilyPlan<
                TFamily,
                TRuntimeProvider,
                TPrimitiveTypes,
                typename ValidateBindings<
                    Planner,
                    ExpandedBindings
                >::Type,
                TResources
            >;

        };


        /// Normalizes every canonical family plan.
        ///
        /// @tparam TPlans Canonical FamilyPlan TypeList.
        /// @tparam TTransportSet Statically configured transport set.
        template<class TPlans, class TTransportSet>
        struct NormalizeFamilyPlans;


        /// Normalizes one concrete family-plan pack.
        template<class... TPlans, class TTransportSet>
        struct NormalizeFamilyPlans<
            TypeList<TPlans...>,
            TTransportSet
        > final {

            /// Normalized family plans in deterministic family order.
            using Type = TypeList<
                typename NormalizeFamilyPlan<
                    TPlans,
                    TTransportSet
                >::Type...
            >;

        };


        /// Extracts the Primitive Type list from one normalized family plan.
        template<class TPlan>
        struct PlanPrimitiveTypes final {

            /// Primitive Types represented by the plan.
            using Type = typename TPlan::PrimitiveTypes;

        };


        /// Extracts the binding list from one normalized family plan.
        template<class TPlan>
        struct PlanBindings final {

            /// Explicit bindings represented by the plan.
            using Type = typename TPlan::Bindings;

        };


        /// Concatenates one projected TypeList from every plan.
        ///
        /// @tparam TPlans Normalized family plan TypeList.
        /// @tparam TProjection Projection returning a nested Type.
        template<
            class TPlans,
            template<class> class TProjection
        >
        struct FlattenPlanLists;


        /// Completes flattening for an empty family-plan list.
        template<template<class> class TProjection>
        struct FlattenPlanLists<
            TypeList<>,
            TProjection
        > final {

            /// Empty flattened list.
            using Type = TypeList<>;

        };


        /// Concatenates the current projected list with recursively flattened remainder.
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


        /// Converts one runtime-provider TypeList to the Primitive System Composition.
        ///
        /// @tparam TProviders Unique canonical runtime provider Types.
        template<class TProviders>
        struct MakeCompositionFromProviders;


        /// Builds the existing System Composition from one concrete provider Type pack.
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


        /// Converts normalized family plans to a Primitive Composition.
        ///
        /// A provider Type may legitimately satisfy more than one family runtime capability.
        /// The Composition therefore contains each runtime provider Type once, while each
        /// FamilyPlan is still independently validated against its exact FamilyRuntime capability.
        ///
        /// @tparam TPlans Normalized family plans.
        template<class TPlans>
        struct MakePrimitiveComposition;


        /// Builds the existing System Composition from unique canonical family runtime providers.
        template<class... TPlans>
        struct MakePrimitiveComposition<
            TypeList<TPlans...>
        > final {

        private:

            /// Runtime provider Types in deterministic normalized family order.
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


        /// Builds the family-neutral ResourcePlanSet retained by normalized Topology output.
        ///
        /// @tparam TPlans Normalized family plans.
        template<class TPlans>
        struct MakeResourcePlanSet;


        /// Retains one independently decomposed ResourcePlan per family.
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
    /// The first template argument is the statically configured TransportSet. Remaining
    /// arguments are family-owned deployment declarations. No runtime object, registry,
    /// allocation, wildcard, or discovery mechanism is created.
    ///
    /// @tparam TTransportSet Statically configured concrete transports.
    /// @tparam TDeployments Family-owned deployment declarations.
    template<class TTransportSet, class... TDeployments>
    struct Topology final {

        static_assert(
            Detail::TransportSetTraits<TTransportSet>::IsValid,
            "Topology requires Primitives::TransportSet as its first template argument"
        );

        static_assert(
            (FamilyDeploymentDeclaration<TDeployments> && ...),
            "Topology entries after TransportSet must be family-owned deployment declarations"
        );

        /// Statically configured concrete transports.
        using Transports = TTransportSet;

        /// Family-owned declarations in application declaration order.
        using Deployments = TypeList<TDeployments...>;

        /// Unique represented families in first-declaration order.
        using Families = typename Detail::DeploymentFamilies<
            Deployments
        >::Type;

        /// Canonical plans emitted by each family's directly associated Planner.
        using FamilyPlans = typename Detail::BuildFamilyPlans<
            Families,
            Deployments,
            Transports
        >::Type;

        /// Fully normalized family plans.
        using NormalizedFamilyPlans = typename Detail::NormalizeFamilyPlans<
            FamilyPlans,
            Transports
        >::Type;

        /// Every deployed Primitive Type across all families.
        using PrimitiveTypes = typename Detail::FlattenPlanLists<
            NormalizedFamilyPlans,
            Detail::PlanPrimitiveTypes
        >::Type;

        static_assert(
            !Detail::HasDuplicateTypes<PrimitiveTypes>::Value,
            "Topology must not deploy the same Primitive Type more than once"
        );

        /// Every explicit normalized transport binding across all families.
        using Bindings = typename Detail::FlattenPlanLists<
            NormalizedFamilyPlans,
            Detail::PlanBindings
        >::Type;

        static_assert(
            !Detail::HasDuplicateTypes<Bindings>::Value,
            "Topology must not contain duplicate explicit bindings across family plans"
        );

        /// Decomposed resource plans retained separately for every family.
        using Resources = typename Detail::MakeResourcePlanSet<
            NormalizedFamilyPlans
        >::Type;

        /// Primitive Domain Composition built from exactly one runtime provider per family.
        using Composition = typename Detail::MakePrimitiveComposition<
            NormalizedFamilyPlans
        >::Type;

        static_assert(
            Composition::IsValid,
            "Normalized Primitive family runtime providers must form a valid System Composition"
        );

    };

} // ESPressio::Primitives
