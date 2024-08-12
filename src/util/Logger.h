#ifndef FILE_UTIL_LOGGER_H
#define FILE_UTIL_LOGGER_H

#include <string>
#include <memory>
#include <fstream>
#include <iomanip>

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Util.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <SDL2/SDL_log.h>

namespace Mineworld {
    using plog::Severity;

    template<class Formatter>
    class LoggerSdlAppender : public plog::IAppender {
        public:
            virtual void write(const plog::Record& record) PLOG_OVERRIDE;
    };

    class LoggerFormatter {
        public:
            static std::string header();
            static std::string format(const plog::Record& record);
    };

    /// TODO: Rewrite with fmt and std::chrono and remove plog dependency
    class Logger {
        public:
            void debug(const std::string& msg);
            void info(const std::string& msg);
            void warn(const std::string& msg);
            void error(const std::string& msg);
            void fatal(const std::string& msg);
            Logger(const std::string& path);
            ~Logger();
        protected:
        private:
            std::shared_ptr<plog::IAppender> sdlAppender;
            std::shared_ptr<plog::IAppender> fileAppender;
            static plog::Logger<PLOG_DEFAULT_INSTANCE_ID> *getInternalLogger();
    };
}
#endif // FILE_UTIL_LOGGER_H
