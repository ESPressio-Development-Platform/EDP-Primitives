#include <ESPressio_Primitives.hpp>

namespace ESPressio::Primitives::Tests::CompileFail {

    struct TestPlanner;

    struct TestFamily final {
        static constexpr PrimitiveFamilyIdentifier Identifier{
            System::TypeAuthorityIdentifier{
                System::TypeAuthorityIdentifier::Storage{0x00U, 0x00U, 0x01U}
            },
            0x01U
        };

        using Planner = TestPlanner;
    };

    struct UnsupportedFieldValue final {
        void* Pointer{nullptr};
    };

    struct NonSerialisablePrimitive final {
        static constexpr System::TypeIdentifier Identifier{
            System::TypeIdentifier::Storage{
                0x00U, 0x00U, 0x01U,
                0x00U, 0x00U, 0x00U, 0x00U, 0x11U
            }
        };

        UnsupportedFieldValue Value{};

        using Fields = System::FieldSet<
            System::FieldBinding<&NonSerialisablePrimitive::Value, 1U>
        >;

        using Family = TestFamily;
    };

    static_assert(System::SchemaType<NonSerialisablePrimitive>);
    static_assert(
        PrimitiveType<NonSerialisablePrimitive>,
        "A schema-bearing Primitive containing an unsupported Field Type must be rejected"
    );

} // ESPressio::Primitives::Tests::CompileFail
