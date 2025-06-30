// Logger/LoggingMacros.h
#pragma once

#include "LoggerManager.h"

// Expected: define LOG_TAG before including this file
#ifndef LOG_TAG
    #error "LOG_TAG must be defined before including LoggingMacros.h"
#endif

#define LOG_INFO(msg)  AnalyticalApproach::Logging::LoggerManager::Get().GetLogger(LOG_TAG)->info(msg)
#define LOG_WARN(msg)  AnalyticalApproach::Logging::LoggerManager::Get().GetLogger(LOG_TAG)->warn(msg)
#define LOG_ERROR(msg) AnalyticalApproach::Logging::LoggerManager::Get().GetLogger(LOG_TAG)->error(msg)
#define LOG_DEBUG(msg) AnalyticalApproach::Logging::LoggerManager::Get().GetLogger(LOG_TAG)->debug(msg)
