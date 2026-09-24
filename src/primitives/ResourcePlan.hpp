#pragma once

#include <cstddef>
#include <type_traits>

#include "TypeList.hpp"

namespace ESPressio::Primitives {

    /// One bounded compile-time resource dimension contributed by a Primitive family.
    ///
    /// Families define semantic resource tags at the narrowest granularity required to
    /// preserve alignment, placement, ownership, lifetime, and other distinctions.
    ///
    /// @tparam TResource Semantic resource-dimension tag.
    /// @tparam TCapacity Maximum statically planned capacity of that dimension.
    template<class TResource, std::size_t TCapacity>
    struct ResourceRequirement final {

        static_assert(
            !std::is_void_v<TResource>,
            "ResourceRequirement requires a concrete resource tag Type"
        );

        /// Semantic resource-dimension tag.
        using Resource = TResource;

        /// Maximum statically planned capacity.
        static constexpr std::size_t Capacity = TCapacity;

    };


    namespace Detail {

        /// Extracts the semantic resource tag from one requirement.
        ///
        /// @tparam TRequirement ResourceRequirement being inspected.
        template<class TRequirement>
        struct ResourceTag;


        /// Extracts the resource tag from a concrete requirement.
        template<class TResource, std::size_t TCapacity>
        struct ResourceTag<
            ResourceRequirement<TResource, TCapacity>
        > final {

            /// Semantic resource tag.
            using Type = TResource;

        };


        /// Default ResourceRequirement metadata for unrelated Types.
        ///
        /// @tparam TType Type being inspected.
        template<class TType>
        struct ResourceRequirementTraits final {

            /// Indicates whether the Type is a ResourceRequirement.
            static constexpr bool IsValid = false;

        };


        /// Metadata for one ResourceRequirement.
        template<class TResource, std::size_t TCapacity>
        struct ResourceRequirementTraits<
            ResourceRequirement<TResource, TCapacity>
        > final {

            /// Indicates that the Type is a valid ResourceRequirement.
            static constexpr bool IsValid = true;

        };

    } // ESPressio::Primitives::Detail


    /// Decomposed bounded resource requirements emitted by one Primitive family planner.
    ///
    /// No universal byte-total or allocation model is inferred. Distinct resource tags
    /// remain distinct dimensions so family/runtime code can preserve semantic differences.
    ///
    /// @tparam TRequirements Bounded resource requirements.
    template<class... TRequirements>
    struct ResourcePlan final {

        static_assert(
            (Detail::ResourceRequirementTraits<TRequirements>::IsValid && ...),
            "ResourcePlan entries must be ResourceRequirement declarations"
        );

        static_assert(
            !Detail::HasDuplicateTypes<
                TypeList<
                    typename Detail::ResourceTag<TRequirements>::Type...
                >
            >::Value,
            "ResourcePlan must not contain duplicate resource-dimension tags"
        );

        /// Requirements in deterministic planner declaration order.
        using Requirements = TypeList<TRequirements...>;

        /// Number of independently bounded resource dimensions.
        static constexpr std::size_t Count = sizeof...(TRequirements);

    };


    /// Family-associated resource plan retained by normalized topology output.
    ///
    /// @tparam TFamily Primitive family owning the resources.
    /// @tparam TPlan ResourcePlan emitted by that family's planner.
    template<class TFamily, class TPlan>
    struct FamilyResources final {

        /// Primitive family owning this plan.
        using Family = TFamily;

        /// Decomposed bounded resource plan.
        using Plan = TPlan;

    };


    /// Family-neutral aggregation of decomposed per-family resource plans.
    ///
    /// @tparam TFamilyResources FamilyResources declarations in normalized family order.
    template<class... TFamilyResources>
    struct ResourcePlanSet final {

        /// Per-family resource plans without implicit cross-family summation.
        using Families = TypeList<TFamilyResources...>;

        /// Number of family resource plans represented.
        static constexpr std::size_t Count = sizeof...(TFamilyResources);

    };

} // ESPressio::Primitives
