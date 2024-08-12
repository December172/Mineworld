#ifndef FILE_EXCEPTION_IOEXCEPTION_H
#define FILE_EXCEPTION_IOEXCEPTION_H

#include <exception>
#include <string>

namespace Mineworld {
    /// @brief   Exception class for IO errors
    /// @details This class is used to throw exceptions when an IO error occurs.
    /// @note    This class should only be used for IO errors that are not related to the Mineworld engine.
    ///          Mostly used in debugging and testing.
    /// @see     Mineworld::RuntimeException
    class IOException : public std::exception {
        public:
            /** Default constructor */
            IOException(const std::string& msg) noexcept;
            virtual const char* what() const noexcept;
        protected:
        private:
            std::string errMsg;
    };
}
#endif // FILE_EXCEPTION_IOEXCEPTION_H
