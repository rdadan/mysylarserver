#ifndef __LOG_APPENDER_H__
#define __LOG_APPENDER_H__

#include <memory>
#include <fstream>
#include <string>
#include "LogLevel.h"
namespace rdadan {

class Logger;
class LogEvent;
class LogFormatter;

// 日志输出器, 基类
class LogAppender {
public:
    using sptr = std::shared_ptr<LogAppender>;
    virtual ~LogAppender() {}
    virtual void logAppender(std::shared_ptr<Logger> logger,
                             LogLevel::Level level,
                             std::shared_ptr<LogEvent> event) = 0;

    void setFormatter(std::shared_ptr<LogFormatter> formater) {
        m_formatter = formater;
    }
    std::shared_ptr<LogFormatter> getFomatter() const { return m_formatter; }

    void setLevel(LogLevel::Level level) { m_level = level; }
    LogLevel::Level getLevel() const { return m_level; }

protected:
    LogLevel::Level m_level =
        LogLevel::DEBUG; // 没有写构造函数，这里直接初始化l
    std::shared_ptr<LogFormatter> m_formatter;
};

// 输出到控制台的 LogAppender
class StdoutAppender : public LogAppender {
public:
    using sptr = std::shared_ptr<StdoutAppender>;
    void logAppender(std::shared_ptr<Logger> logger, LogLevel::Level level,
                     std::shared_ptr<LogEvent> event) override;
};
// 输出到文件的 LogAppender
class FileAppender : public LogAppender {
public:
    using sptr = std::shared_ptr<FileAppender>;
    FileAppender(const std::string filename);
    void logAppender(std::shared_ptr<Logger> logger, LogLevel::Level level,
                     std::shared_ptr<LogEvent> event) override;

    bool reopenFile();

private:
    std::string m_filename;
    std::ofstream m_filestream;
};

} // namespace rdadan

#endif