#include <ESPressio_Primitives.hpp>

namespace {

    struct Transport final {};

    using InvalidTransports = ESPressio::Primitives::TransportSet<
        Transport,
        Transport
    >;

    static_assert(sizeof(InvalidTransports) > 0U);

} // anonymous namespace

int main() {
    return 0;
}
