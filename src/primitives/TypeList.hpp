#pragma once

#include <cstddef>
#include <type_traits>

namespace ESPressio::Primitives {

    /// Deterministic compile-time list used by Primitive extension contracts.
    ///
    /// @tparam TTypes Types represented by this list in declaration order.
    template<class... TTypes>
    struct TypeList final {

        /// Number of Types represented by this list.
        static constexpr std::size_t Count = sizeof...(TTypes);

        /// Indicates whether the list contains no Types.
        static constexpr bool IsEmpty = Count == 0U;

        /// Indicates whether the supplied Type occurs in this list.
        ///
        /// @tparam TType Type whose presence is queried.
        template<class TType>
        static constexpr bool Contains = (std::is_same_v<TType, TTypes> || ...);

    };


    namespace Detail {

        /// Concatenates two compile-time TypeList declarations.
        ///
        /// @tparam TLeft First TypeList.
        /// @tparam TRight Second TypeList.
        template<class TLeft, class TRight>
        struct ConcatTypeLists;


        /// Concatenates the represented Type packs while preserving declaration order.
        template<class... TLeft, class... TRight>
        struct ConcatTypeLists<
            TypeList<TLeft...>,
            TypeList<TRight...>
        > final {

            /// Concatenated Type list.
            using Type = TypeList<TLeft..., TRight...>;

        };


        /// Appends one Type only when it is absent from the represented list.
        ///
        /// @tparam TList Existing TypeList.
        /// @tparam TType Candidate Type.
        template<class TList, class TType>
        struct AppendUniqueType;


        /// Preserves the existing order and conditionally appends the candidate Type.
        template<class... TTypes, class TType>
        struct AppendUniqueType<
            TypeList<TTypes...>,
            TType
        > final {

            /// Type list containing the candidate at most once.
            using Type = std::conditional_t<
                (std::is_same_v<TType, TTypes> || ...),
                TypeList<TTypes...>,
                TypeList<TTypes..., TType>
            >;

        };


        /// Removes duplicate Types from a TypeList while preserving first-occurrence order.
        ///
        /// @tparam TInput Input TypeList.
        /// @tparam TAccumulated Unique Types already observed.
        template<class TInput, class TAccumulated = TypeList<>>
        struct UniqueTypeList;


        /// Completes unique-Type collection when no input Types remain.
        template<class... TAccumulated>
        struct UniqueTypeList<
            TypeList<>,
            TypeList<TAccumulated...>
        > final {

            /// Unique Type list in first-occurrence order.
            using Type = TypeList<TAccumulated...>;

        };


        /// Inspects one input Type and continues unique-Type collection.
        template<class TFirst, class... TRest, class... TAccumulated>
        struct UniqueTypeList<
            TypeList<TFirst, TRest...>,
            TypeList<TAccumulated...>
        > final {

            /// Accumulated list after conditionally appending the current Type.
            using Next = typename AppendUniqueType<
                TypeList<TAccumulated...>,
                TFirst
            >::Type;

            /// Final unique Type list.
            using Type = typename UniqueTypeList<
                TypeList<TRest...>,
                Next
            >::Type;

        };


        /// Determines whether one TypeList contains duplicate Types.
        ///
        /// @tparam TList TypeList being inspected.
        template<class TList>
        struct HasDuplicateTypes;


        /// Reports whether any represented Type appears more than once.
        template<class... TTypes>
        struct HasDuplicateTypes<TypeList<TTypes...>> final {

            /// Unique form of the represented Types.
            using Unique = typename UniqueTypeList<
                TypeList<TTypes...>
            >::Type;

            /// Indicates whether duplicate Types were present.
            static constexpr bool Value =
                Unique::Count != sizeof...(TTypes);

        };

    } // ESPressio::Primitives::Detail

} // ESPressio::Primitives
