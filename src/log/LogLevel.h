#ifndef __LOH_LEVEL_H__
#define __LOH_LEVEL_H__

namespace rdadan {

// 日志级别
class LogLevel {
public:
    enum Level {
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };

    static const char *ToString(LogLevel::Level level);
};

} // namespace rdadan
#endif