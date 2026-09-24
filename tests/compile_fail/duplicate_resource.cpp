#include <ESPressio_Primitives.hpp>

namespace {

    struct QueueSlots final {};

    using InvalidPlan = ESPressio::Primitives::ResourcePlan<
        ESPressio::Primitives::ResourceRequirement<
            QueueSlots,
            4U
        >,
        ESPressio::Primitives::ResourceRequirement<
            QueueSlots,
            8U
        >
    >;

    static_assert(sizeof(InvalidPlan) > 0U);

} // anonymous namespace

int main() {
    return 0;
}
