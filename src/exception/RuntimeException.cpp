#include "RuntimeException.h"
#include "Mineworld.h"

namespace Mineworld {
    RuntimeException::RuntimeException(const std::string& msg, const Severity& level) : errMsg(msg), errLevel(level) {
        Logger logger = Mineworld::getLogger();
        switch (level) {
        case Severity::debug:
            logger.debug(msg);
            break;
        case Severity::info:
            logger.info(msg);
            break;
        case Severity::warning:
            logger.warn(msg);
            break;
        case Severity::error:
            logger.error(msg);
            break;
        case Severity::fatal:
            logger.fatal(msg);
            break;
        default:
            break;
        }
    }

    const char * RuntimeException::what() const noexcept {
        return errMsg.data();
    }
}
