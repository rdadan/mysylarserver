#include "LogEvent.h"
#include "Logger.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <string>

namespace rdadan {

LogEvent::LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
                   const char *file, int32_t line, uint32_t elapse,
                   uint32_t thread_id, uint32_t fiber_id, uint64_t time)
    : m_file(file),
      m_line(line),
      m_elapse(elapse),
      m_threadId(thread_id),
      m_fiberId(fiber_id),
      m_time(time),
      m_logger(logger),
      m_level(level) {
}

void LogEvent::format(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    format(fmt, vl);
    va_end(vl);
}
void LogEvent::format(const char *fmt, va_list vl) {
    char *buf = nullptr;
    int len = vasprintf(&buf, fmt, vl);
    if (len != -1) {
        m_ss << std::string(buf, len);
        free(buf);
    }
}

LogEventWrap::LogEventWrap(LogEvent::sptr event) : m_event(event) {
    // std::cout << "LogEventWrap()" << std::endl;
}

LogEventWrap::~LogEventWrap() {
    // std::cout << "~LogEventWrap()" << std::endl;

    m_event->getLogger()->log(m_event->getLevel(), m_event);
}

} // namespace rdadan