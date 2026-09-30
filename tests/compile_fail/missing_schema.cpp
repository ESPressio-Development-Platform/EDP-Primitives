#include <ESPressio_Primitives.hpp>

namespace ESPressio::Primitives::Tests::CompileFail {

    struct TestPlanner;

    struct TestFamily final {
        static constexpr PrimitiveFamilyIdentifier Identifier{
            System::TypeAuthorityIdentifier{
                System::TypeAuthorityIdentifier::Storage{
                    0x00U,
                    0x00U,
                    0x01U
                }
            },
            0x01U
        };

        using Planner = TestPlanner;
    };

    struct MissingSchemaPrimitive final {
        static constexpr System::TypeIdentifier Identifier{
            System::TypeIdentifier::Storage{
                0x00U,
                0x00U,
                0x01U,
                0x00U,
                0x00U,
                0x00U,
                0x00U,
                0x10U
            }
        };

        using Family = TestFamily;
    };

    static_assert(
        PrimitiveType<MissingSchemaPrimitive>,
        "A Primitive lacking System schema metadata must be rejected"
    );

} // ESPressio::Primitives::Tests::CompileFail
