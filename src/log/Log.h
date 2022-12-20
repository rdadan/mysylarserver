#ifndef __LOH_H__
#define __LOH_H__
#include "LogLevel.h"

#define LOG_LEVEL(logger, level)                                                    \
    if (logger->getLevel() <= level)                                                \
    rdadan::LogEventWrap(                                                           \
        rdadan::LogEvent::sptr(new rdadan::LogEvent(                                \
            logger, level, __FILE__, __LINE__, 0, rdadan::GetThreadID(),            \
            rdadan::GetFiberID(), time(0))))                                        \
        .getSS()

#define LOG_DEBUG(logger) LOG_LEVEL(logger, rdadan::LogLevel::DEBUG)
#define LOG_INFO(logger) LOG_LEVEL(logger, rdadan::LogLevel::INFO)
#define LOG_WARN(logger) LOG_LEVEL(logger, rdadan::LogLevel::WARN)
#define LOG_ERROR(logger) LOG_LEVEL(logger, rdadan::LogLevel::ERROR)
#define LOG_FATAL(logger) LOG_LEVEL(logger, rdadan::LogLevel::FATAL)

#define LOG_FMT_LEVEL(logger, level, fmt, ...)                                      \
    if (logger->getLevel() <= level)                                                \
    rdadan::LogEventWrap(                                                           \
        rdadan::LogEvent::sptr(new rdadan::LogEvent(                                \
            logger, level, __FILE__, __LINE__, 0, rdadan::GetThreadID(),            \
            rdadan::GetFiberID(), time(0))))                                        \
        .getLogEvent()                                                              \
        ->format(fmt, __VA_ARGS__)

#define LOG_FMT_DEBUG(logger, fmt, ...)                                             \
    LOG_FMT_LEVEL(logger, rdadan::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define LOG_FMT_INFO(logger, fmt, ...)                                              \
    LOG_FMT_LEVEL(logger, rdadan::LogLevel::INFO, fmt, __VA_ARGS__)
#define LOG_FMT_WARN(logger, fmt, ...)                                              \
    LOG_FMT_LEVEL(logger, rdadan::LogLevel::WARN, fmt, __VA_ARGS__)
#define LOG_FMT_ERROR(logger, fmt, ...)                                             \
    LOG_FMT_LEVEL(logger, rdadan::LogLevel::ERROR, fmt, __VA_ARGS__)
#define LOG_FMT_FATAL(logger, fmt, ...)                                             \
    LOG_FMT_LEVEL(logger, rdadan::LogLevel::FATAL, fmt, __VA_ARGS__)

#define LOG_ROOT() rdadan::LoggerMgr::GetInstance()->getRoot()

#endif