#include "LoggerManager.h"

namespace AnalyticalApproach::Logging
{
    LoggerManager& LoggerManager::Get()
    {
        static LoggerManager instance;
        return instance;
    }

    void LoggerManager::Init()
    {
        // You could pre-create core loggers here if desired
        // Or leave it empty and rely on GetLogger auto-creation
    }

    std::shared_ptr<spdlog::logger> LoggerManager::GetLogger(const std::string& tag)
    {
        auto it = m_loggers.find(tag);
        if (it != m_loggers.end())
        {
            return it->second;
        }

        // Create default sinks
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        // Format: [Tag::Level] message
        console_sink->set_pattern("[%n::%^%l%$] %v");

        std::vector<spdlog::sink_ptr> sinks = { console_sink };
        auto logger = std::make_shared<spdlog::logger>(tag, sinks.begin(), sinks.end());
        logger->set_level(spdlog::level::trace); // default to all logs enabled

        spdlog::register_logger(logger);
        m_loggers[tag] = logger;

        return logger;
    }

    void LoggerManager::SetLogLevel(const std::string& tag, spdlog::level::level_enum level)
    {
        auto logger = GetLogger(tag);
        if (logger)
        {
            logger->set_level(level);
        }
    }

    void LoggerManager::SetFileSink(const std::string& tag, const std::string& filepath, spdlog::level::level_enum level)
    {
        auto logger = GetLogger(tag);
        if (!logger) return;

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filepath, true);
        file_sink->set_pattern("[%n::%l] %v");
        file_sink->set_level(level);

        logger->sinks().push_back(file_sink);
    }
}
