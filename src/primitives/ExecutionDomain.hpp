#pragma once

#include <type_traits>

namespace ESPressio::Primitives::ExecutionDomain {

    /// Selects execution only in the local Primitive domain.
    struct LocalOnly final {};

    /// Selects execution only in an external remote Primitive domain.
    struct RemoteOnly final {};

    /// Selects independent execution in both local and external remote Primitive domains.
    struct LocalAndRemote final {};

    /// Identifies one supported compile-time Primitive execution-domain scope tag.
    /// @tparam TScope Candidate execution-domain scope Type.
    template<class TScope>
    concept Scope =
        std::is_same_v<std::remove_cvref_t<TScope>, LocalOnly> ||
        std::is_same_v<std::remove_cvref_t<TScope>, RemoteOnly> ||
        std::is_same_v<std::remove_cvref_t<TScope>, LocalAndRemote>;

} // ESPressio::Primitives::ExecutionDomain
