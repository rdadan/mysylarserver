#ifndef __LOG_EVENT_H__
#define __LOG_EVENT_H__

#include <memory>
#include <string>
#include <sstream>
#include "LogLevel.h"
namespace rdadan {

class Logger;
// 日志事件
class LogEvent {
public:
    using sptr = std::shared_ptr<LogEvent>;

    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char *file,
             int32_t m_line, uint32_t elapse, uint32_t thread_id, uint32_t fiber_id,
             uint64_t time);

    const char *getFile() const { return m_file; }
    int32_t getLine() const { return m_line; }
    uint32_t getElapse() const { return m_elapse; }
    uint32_t getThreadID() const { return m_threadId; }
    uint32_t getFiberID() const { return m_fiberId; }
    uint32_t getTime() const { return m_time; }

    std::string getContent() const { return m_ss.str(); }
    std::shared_ptr<Logger> getLogger() const { return m_logger; }
    LogLevel::Level getLevel() const { return m_level; }

    std::stringstream &getSS() { return m_ss; }
    // 格式化写入日志
    void format(const char *fmt, ...);
    void format(const char *fmt, va_list vl);

private:
    const char *m_file = nullptr; //文件名
    int32_t m_line = 0;           //行号
    uint32_t m_elapse = 0;        // 程序启动到现在的毫秒数
    uint32_t m_threadId = 0;      //线程id
    uint32_t m_fiberId = 0;       //协程id
    uint32_t m_time = 0;          //时间戳
    std::stringstream m_ss;       //日志内容

    std::shared_ptr<Logger> m_logger;
    LogLevel::Level m_level;
};

class LogEventWrap {
public:
    LogEventWrap(LogEvent::sptr event);
    ~LogEventWrap();
    LogEvent::sptr getLogEvent() const { return m_event; }
    std::stringstream &getSS() { return m_event->getSS(); }

private:
    LogEvent::sptr m_event;
};
} // namespace rdadan

#endif