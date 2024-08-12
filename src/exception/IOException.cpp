#include "IOException.h"

namespace Mineworld {
    IOException::IOException(const std::string& msg) noexcept {
        errMsg = msg;
    }

    const char* IOException::what() const noexcept {
        return errMsg.data();
    }
}
