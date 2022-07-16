#ifndef __LOHGGER_H__
#define __LOHGGER_H__

#include <iostream>
#include <memory>
#include <string>
#include <list>
#include <map>
#include "../SingleTon.h"
#include "LogLevel.h"
#include "LogEvent.h"
#include "LogAppender.h"
#include "LogFormatter.h"
#include "../util.h"

namespace rdadan {

// 日志器
class Logger : public std::enable_shared_from_this<Logger> {
public:
    using sptr = std::shared_ptr<Logger>;
    Logger(const std::string &name = "root");
    void addApppender(LogAppender::sptr appender);
    void delApppender(LogAppender::sptr appender);

    void log(LogLevel::Level level, LogEvent::sptr event);
    void debug(LogEvent::sptr event);
    void info(LogEvent::sptr event);
    void warn(LogEvent::sptr event);
    void fatal(LogEvent::sptr event);
    void error(LogEvent::sptr event);

    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level level) { m_level = level; }
    std::string getName() const { return m_name; }

private:
    std::string m_name;      // 日志名称
    LogLevel::Level m_level; // 只有满足这个日志级别的才会被输出
    std::list<LogAppender::sptr> m_appenders; // Appender集合
    LogFormatter::sptr m_formatter;
};

class LoggerManager {
public:
    LoggerManager();
    Logger::sptr getLogger(const std::string &name);
    void init();
    Logger::sptr getRoot() const { return m_root; }

private:
    std::map<std::string, Logger::sptr> m_loggers;
    Logger::sptr m_root;
};

using LoggerMgr = rdadan::SingleTon<LoggerManager>;
} // namespace rdadan
#endif