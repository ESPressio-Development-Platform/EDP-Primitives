#include <ESPressio_Primitives.hpp>

namespace Test {

    struct Planner;

    struct InvalidFamily final {
        static constexpr ESPressio::Primitives::PrimitiveFamilyIdentifier Identifier{
            ESPressio::System::TypeAuthorityIdentifier{
                ESPressio::System::TypeAuthorityIdentifier::Storage{
                    0x00U,
                    0x00U,
                    0x00U
                }
            },
            0x01U
        };
        using Planner = Test::Planner;
    };

    static_assert(
        ESPressio::Primitives::PrimitiveFamilyType<InvalidFamily>,
        "The compile-fail fixture intentionally requires an invalid family to satisfy the concept"
    );

} // Test

int main() {
    return 0;
}
