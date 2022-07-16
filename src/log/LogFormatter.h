#ifndef __LOG_FORMATTER_H__
#define __LOG_FORMATTER_H__

#include <memory>
#include <string>
#include <vector>
#include "LogLevel.h"
namespace rdadan {

class Logger;
class LogEvent;

//日志格式器
//默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
class LogFormatter {
public:
    using sptr = std::shared_ptr<LogFormatter>;
    LogFormatter(const std::string &strFomrat);
    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level,
                       std::shared_ptr<LogEvent> event);
    // 初始化 解析日志模板
    void init();

    class FormatItem {
    public:
        using sptr = std::shared_ptr<FormatItem>;
        virtual ~FormatItem() {}
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger,
                            LogLevel::Level level,
                            std::shared_ptr<LogEvent> event) = 0;
    };

private:
    std::string m_strFormat;                     // 日志模板
    std::vector<FormatItem::sptr> m_formatItems; // 解析好的各种日志格式
};
} // namespace rdadan

#endif