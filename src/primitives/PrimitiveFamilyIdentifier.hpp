#pragma once

#include <cstdint>

#include <ESPressio_System.hpp>

namespace ESPressio::Primitives {

    /// Stable identity of one Primitive family.
    ///
    /// The packed 32-bit representation contains the globally governed 24-bit
    /// System Type Authority in the upper bits and an authority-local 8-bit
    /// Primitive-family value in the lower bits. Zero is invalid in either component.
    class PrimitiveFamilyIdentifier final {
    private:

        // Packed family identity.

        /// Exact packed 24-bit Authority plus 8-bit authority-local family value.
        std::uint32_t _value{};

    public:

        // Family identity metadata.

        /// Exact retained family identity width in bytes.
        static constexpr std::uint8_t Size = 4U;


        // Construction.

        /// Creates the Invalid/Unspecified zero family identity.
        constexpr PrimitiveFamilyIdentifier() noexcept = default;

        /// Creates a family identity from its strong Authority and local family value.
        ///
        /// @param authority Globally governed System Type Authority.
        /// @param localValue Authority-local Primitive-family value.
        constexpr PrimitiveFamilyIdentifier(
            const System::TypeAuthorityIdentifier& authority,
            std::uint8_t localValue
        ) noexcept :
            _value(
                (authority.Value() << 8U) |
                static_cast<std::uint32_t>(localValue)
            ) {}


        // Value access.

        /// Returns the exact packed 32-bit representation.
        constexpr std::uint32_t Value() const noexcept {
            return _value;
        }

        /// Returns the strong System Type Authority component.
        constexpr System::TypeAuthorityIdentifier Authority() const noexcept {
            return System::TypeAuthorityIdentifier(
                System::TypeAuthorityIdentifier::Storage{
                    static_cast<std::uint8_t>((_value >> 24U) & 0xFFU),
                    static_cast<std::uint8_t>((_value >> 16U) & 0xFFU),
                    static_cast<std::uint8_t>((_value >> 8U) & 0xFFU)
                }
            );
        }

        /// Returns the authority-local 8-bit Primitive-family value.
        constexpr std::uint8_t LocalValue() const noexcept {
            return static_cast<std::uint8_t>(_value & 0xFFU);
        }

        /// Indicates whether both family-identity components are non-zero.
        constexpr bool IsValid() const noexcept {
            return Authority().IsValid() && LocalValue() != 0U;
        }

        /// Indicates whether both family-identity components are non-zero.
        constexpr explicit operator bool() const noexcept {
            return IsValid();
        }


        // Comparison.

        /// Compares packed family identities for equality.
        ///
        /// @param other Family identity to compare with this value.
        constexpr bool operator ==(
            const PrimitiveFamilyIdentifier& other
        ) const noexcept {
            return _value == other._value;
        }

        /// Compares packed family identities for inequality.
        ///
        /// @param other Family identity to compare with this value.
        constexpr bool operator !=(
            const PrimitiveFamilyIdentifier& other
        ) const noexcept {
            return !(*this == other);
        }

        /// Provides deterministic numeric ordering of the canonical packed identity.
        ///
        /// @param other Family identity to compare with this value.
        constexpr bool operator <(
            const PrimitiveFamilyIdentifier& other
        ) const noexcept {
            return _value < other._value;
        }

    };


    static_assert(
        sizeof(PrimitiveFamilyIdentifier) == PrimitiveFamilyIdentifier::Size,
        "PrimitiveFamilyIdentifier must remain an exact 4-byte semantic value"
    );

} // ESPressio::Primitives
