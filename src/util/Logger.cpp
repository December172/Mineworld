#include "Logger.h"

namespace Mineworld {
    using plog::Severity;
    plog::Logger<PLOG_DEFAULT_INSTANCE_ID> *Logger::getInternalLogger() {
        return plog::get();
    }

    Logger::Logger(const std::string& path) {
        std::ofstream outfile(path.data(), std::ios::out | std::ios::trunc);
        if (outfile) {
            outfile.close();
        }
        plog::IAppender *tmpSdlAppender, *tmpFileAppender;
        tmpSdlAppender = new LoggerSdlAppender<LoggerFormatter>();
        tmpFileAppender = new plog::RollingFileAppender<LoggerFormatter>(path.data(), 0, 0);
        sdlAppender.reset(tmpSdlAppender);
        fileAppender.reset(tmpFileAppender);
        plog::init(Severity::debug, sdlAppender.get());
        getInternalLogger()->addAppender(tmpFileAppender);
    }

    void Logger::debug(const std::string& msg) {
        PLOGD << msg;
    }

    void Logger::info(const std::string& msg) {
        PLOGI << msg;
    }

    void Logger::warn(const std::string& msg) {
        PLOGW << msg;
    }

    void Logger::error(const std::string& msg) {
        PLOGE << msg;
    }

    void Logger::fatal(const std::string& msg) {
        PLOGF << msg;
    }

    Logger::~Logger() {}

    std::string LoggerFormatter::header() {
        return std::string();
    }

    std::string LoggerFormatter::format(const plog::Record& record) {
        std::stringstream ss;
        std::tm t;
        plog::util::localtime_s(&t, &record.getTime().time);
        std::string second = (t.tm_sec < 10)    ? "0" + std::to_string(t.tm_sec) : std::to_string(t.tm_sec);
        std::string minute = (t.tm_min < 10)   ? "0" + std::to_string(t.tm_min) : std::to_string(t.tm_min);
        std::string hour     = (t.tm_hour < 10) ? "0" + std::to_string(t.tm_hour) : std::to_string(t.tm_hour);
        std::string month  = (t.tm_mon < 10) ? "0" + std::to_string(t.tm_mon + 1) : std::to_string(t.tm_mon + 1);
        std::string day       = (t.tm_mday < 10) ? "0" + std::to_string(t.tm_mday) : std::to_string(t.tm_mday);
        ss << "[" << t.tm_year + 1900 << "-" << month << "-" << day << " " << hour << ":" << minute << ":" << second << "]"
           << "[main/" << plog::severityToString(record.getSeverity()) << "] "
           << record.getMessage() << "\n";
        return ss.str();
    }

    template<class LoggerFormatter>
    void LoggerSdlAppender<LoggerFormatter>::write(const plog::Record& record) {
        Severity severity = record.getSeverity();
        /// Could we do this more efficiently?
        switch(severity) {
            case Severity::fatal:
                SDL_LogCritical(0, (LoggerFormatter::format(record).data()));
                break;
            case Severity::error:
                SDL_LogError(0, (LoggerFormatter::format(record).data()));
                break;
            case Severity::warning:
                SDL_LogWarn(0,  (LoggerFormatter::format(record).data()));
                break;
            case Severity::info:
                SDL_LogInfo(0, (LoggerFormatter::format(record).data()));
                break;
            case Severity::debug:
                SDL_LogDebug(0, (LoggerFormatter::format(record).data()));
                break;
            default:
                break;
        }
    }
}
