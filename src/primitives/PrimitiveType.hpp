#pragma once

#include <type_traits>

#include "PrimitiveFamilyIdentifier.hpp"

namespace ESPressio::Primitives {

    namespace Detail {

        /// Indicates whether a Type declares a directly readable static family Identifier.
        ///
        /// @tparam TFamily Primitive family tag being inspected.
        template<class TFamily>
        concept HasPrimitiveFamilyIdentifierMember = requires {
            TFamily::Identifier;
        };


        /// Determines whether a family Identifier uses the exact PrimitiveFamilyIdentifier domain.
        ///
        /// @tparam TFamily Primitive family tag being inspected.
        template<class TFamily>
        consteval bool HasExactPrimitiveFamilyIdentifier() {
            if constexpr (!HasPrimitiveFamilyIdentifierMember<TFamily>) return false;

            return std::is_same_v<
                std::remove_cv_t<decltype(TFamily::Identifier)>,
                PrimitiveFamilyIdentifier
            >;
        }

        /// Determines whether a family Identifier is a valid constant expression.
        ///
        /// @tparam TFamily Primitive family tag being inspected.
        template<class TFamily>
        consteval bool HasConstantPrimitiveFamilyIdentifier() {
            if constexpr (!HasExactPrimitiveFamilyIdentifier<TFamily>()) return false;

            return requires {
                typename std::bool_constant<TFamily::Identifier.IsValid()>;
            };
        }

        /// Determines whether a family declares its canonical Planner association.
        ///
        /// @tparam TFamily Primitive family tag being inspected.
        template<class TFamily>
        concept HasPrimitiveFamilyPlanner = requires {
            typename TFamily::Planner;
        };

        /// Determines whether a Type satisfies the complete Primitive-family tag contract.
        ///
        /// @tparam TFamily Primitive family tag being inspected.
        template<class TFamily>
        consteval bool IsPrimitiveFamilyType() {
            if constexpr (!HasConstantPrimitiveFamilyIdentifier<TFamily>()) return false;
            if constexpr (!HasPrimitiveFamilyPlanner<TFamily>) return false;

            return TFamily::Identifier.IsValid();
        }

        /// Reads one Primitive-family Identifier with focused contract diagnostics.
        ///
        /// @tparam TFamily Primitive family tag whose Identifier is being read.
        template<class TFamily>
        consteval PrimitiveFamilyIdentifier ReadPrimitiveFamilyIdentifier() {
            static_assert(
                HasPrimitiveFamilyIdentifierMember<TFamily>,
                "Primitive family Types must declare a static Identifier member"
            );

            if constexpr (!HasPrimitiveFamilyIdentifierMember<TFamily>) {
                return PrimitiveFamilyIdentifier{};
            } else {
                static_assert(
                    HasExactPrimitiveFamilyIdentifier<TFamily>(),
                    "Primitive family Identifier must have the exact Primitives::PrimitiveFamilyIdentifier Type"
                );

                if constexpr (!HasExactPrimitiveFamilyIdentifier<TFamily>()) {
                    return PrimitiveFamilyIdentifier{};
                } else {
                    static_assert(
                        HasConstantPrimitiveFamilyIdentifier<TFamily>(),
                        "Primitive family Identifier must be a constant-expression declaration"
                    );

                    if constexpr (!HasConstantPrimitiveFamilyIdentifier<TFamily>()) {
                        return PrimitiveFamilyIdentifier{};
                    } else {
                        static_assert(
                            TFamily::Identifier.IsValid(),
                            "Primitive family Identifier must contain non-zero Authority and local-family components"
                        );

                        static_assert(
                            HasPrimitiveFamilyPlanner<TFamily>,
                            "Primitive family Types must associate exactly one canonical Planner through Family::Planner"
                        );

                        return TFamily::Identifier;
                    }
                }
            }
        }


        /// Determines whether a semantic Type declares a nested Primitive family tag.
        ///
        /// @tparam TType Semantic Type being inspected.
        template<class TType>
        concept HasPrimitiveFamilyMember = requires {
            typename TType::Family;
        };


        /// Reads one Primitive family tag with focused contract diagnostics.
        ///
        /// @tparam TType Semantic Primitive Type whose family is being read.
        template<class TType>
        struct ReadPrimitiveFamily final {

            static_assert(
                System::IdentifiedType<TType>,
                "Primitive Types must satisfy System::IdentifiedType"
            );

            static_assert(
                HasPrimitiveFamilyMember<TType>,
                "Primitive Types must declare a nested Family Type"
            );

            /// Family Type declared directly by the semantic Primitive.
            using Type = typename TType::Family;

            static_assert(
                IsPrimitiveFamilyType<Type>(),
                "Primitive Type Family must satisfy the Primitive-family tag contract"
            );

        };

    } // ESPressio::Primitives::Detail


    /// Predicate identifying valid Primitive-family tag Types.
    ///
    /// @tparam TFamily Primitive family tag being validated.
    template<class TFamily>
    concept PrimitiveFamilyType = Detail::IsPrimitiveFamilyType<TFamily>();


    /// Predicate identifying semantic Types carrying universal identity and Primitive-family classification.
    ///
    /// @tparam TType Semantic Primitive Type being validated.
    template<class TType>
    concept PrimitiveType =
        System::IdentifiedType<TType> &&
        Detail::HasPrimitiveFamilyMember<TType> &&
        PrimitiveFamilyType<typename TType::Family>;


    /// Canonical compile-time reader for one semantic Primitive's family tag.
    ///
    /// @tparam TType Semantic Primitive Type whose family is requested.
    template<class TType>
    using PrimitiveFamilyOf = typename Detail::ReadPrimitiveFamily<TType>::Type;


    /// Canonical compile-time reader for one Primitive family's stable numeric identity.
    ///
    /// @tparam TType Semantic Primitive Type whose family identifier is requested.
    template<class TType>
    inline constexpr PrimitiveFamilyIdentifier PrimitiveFamilyIdentifierOf =
        Detail::ReadPrimitiveFamilyIdentifier<
            PrimitiveFamilyOf<TType>
        >();

} // ESPressio::Primitives
