#ifndef FILE_EXCEPTION_RUNTIMEEXCEPTION_H
#define FILE_EXCEPTION_RUNTIMEEXCEPTION_H

#include <exception>
#include <string>

#include "util/Logger.h"

namespace Mineworld {
    /// @brief   A custom exception class for runtime errors.
    /// @details This class is used to throw runtime errors in the Mineworld program. Provides logging functionality.
    /// @note    This class should only be used for native runtime errors (such as shader compilation errors)
    ///          not for errors caused by IO or code errors.
    /// @see     Mineworld::IOException
    class RuntimeException : public std::exception {
        public:
            /** Default constructor */
            RuntimeException(const std::string& msg, const Severity& level);
            virtual const char* what() const noexcept override;
        protected:
        private:
            std::string errMsg;
            Severity errLevel;
    };
}

#endif // FILE_EXCEPTION_RUNTIMEEXCEPTION_H
