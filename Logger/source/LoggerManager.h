#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace AnalyticalApproach::Logging
{
    class LoggerManager
    {
    public:
        static LoggerManager& Get();

        void Init(); // Call this once during app startup

        std::shared_ptr<spdlog::logger> GetLogger(const std::string& tag);

        void SetLogLevel(const std::string& tag, spdlog::level::level_enum level);
        void SetFileSink(const std::string& tag, const std::string& filepath, spdlog::level::level_enum level);

    private:
        LoggerManager() = default;
        ~LoggerManager() = default;
        LoggerManager(const LoggerManager&) = delete;
        LoggerManager& operator=(const LoggerManager&) = delete;

        std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> m_loggers;
    };
}
