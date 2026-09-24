#include <ESPressio_Primitives.hpp>

#include "../TestSupport.hpp"

namespace {

    namespace Support = ESPressio::Primitives::Tests::Support;

    using InvalidTopology = ESPressio::Primitives::Topology<
        ESPressio::Primitives::TransportSet<
            Support::TransportB
        >,
        Support::DeploymentOne
    >;

    static_assert(InvalidTopology::Bindings::Count > 0U);

} // anonymous namespace

int main() {
    return 0;
}
