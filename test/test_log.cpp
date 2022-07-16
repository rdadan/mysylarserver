#include "../src/log/Log.h"
#include "../src/log/Logger.h"
#include "../src/log/LogLevel.h"

#include <iostream>
using namespace rdadan;
int main(int argc, char *argv[]) {
    // 初始化logger的时候，也初始化了logformatter和 loglevel
    Logger::sptr logger(new Logger);

    // 添加标准输入日志
    logger->addApppender(LogAppender::sptr(new StdoutAppender));
    // 添加文件日志
    FileAppender::sptr file_appender(new FileAppender("./log.txt"));
    // 设置文件日志格式
    // LogFormatter::sptr fmt(new LogFormatter("%d%T%p%T%m%n"));
    // file_appender->setFormatter(fmt);
    logger->addApppender(file_appender);

    std::cout << "test  log " << std::endl;
    LOG_INFO(logger) << " LOG_INFO";
    LOG_ERROR(logger) << "test macro LOG_ERROR ";

    LOG_FMT_ERROR(logger, "test macro LOG_FMT_ERROR %s, %d", "s1", 111);
    auto log = LoggerMgr::getInstance()->getLogger("xxx");
    LOG_INFO(log) << "xxx";
    return 0;
}
