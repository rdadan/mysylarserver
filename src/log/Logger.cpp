#include "Logger.h"
#include <iostream>
namespace rdadan {

// 日志器
Logger::Logger(const std::string &name)
    : m_name(name), m_level(LogLevel::DEBUG) {
    m_formatter.reset(new LogFormatter(
        "%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
}

void Logger::addApppender(LogAppender::sptr appender) {
    if (!appender->getFomatter())
        appender->setFormatter(m_formatter);
    m_appenders.push_back(appender);
}
void Logger::delApppender(LogAppender::sptr appender) {
    for (auto it = m_appenders.begin(); it != m_appenders.end(); it++) {
        if (*it == appender) {
            m_appenders.erase(it);
            break;
        }
    }
}

void Logger::log(LogLevel::Level level, LogEvent::sptr event) {
    if (level >= m_level) {
        auto currrent_log = shared_from_this();
        for (auto &p : m_appenders) {
            p->logAppender(currrent_log, level, event);
        }
    }
}

void Logger::debug(LogEvent::sptr event) {
    log(LogLevel::DEBUG, event);
}
void Logger::info(LogEvent::sptr event) {
    log(LogLevel::INFO, event);
}
void Logger::warn(LogEvent::sptr event) {
    log(LogLevel::WARN, event);
}
void Logger::fatal(LogEvent::sptr event) {
    log(LogLevel::FATAL, event);
}
void Logger::error(LogEvent::sptr event) {
    log(LogLevel::ERROR, event);
}

LoggerManager::LoggerManager() {
    m_root.reset(new Logger);
    m_root->addApppender(LogAppender::sptr(new StdoutAppender));
}

Logger::sptr LoggerManager::getLogger(const std::string &name) {
    auto it = m_loggers.find(name);
    return it == m_loggers.end() ? m_root : it->second;
}

} // namespace rdadan