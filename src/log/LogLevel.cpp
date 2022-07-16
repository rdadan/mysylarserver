#include "LogLevel.h"
namespace rdadan {

// 日志级别转为文本
const char *LogLevel::ToString(LogLevel::Level level) {
    switch (level) {
#define XX(name)                                                                    \
    case LogLevel::name: return #name; break;

        XX(DEBUG);
        XX(INFO);
        XX(WARN);
        XX(ERROR);
        XX(FATAL);
#undef XX
    default: return "UNKNOW";
    }
    return "UNKNOW";
}

} // namespace rdadan