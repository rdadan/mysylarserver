
#include "LogAppender.h"
#include "LogFormatter.h"
#include <iostream>

namespace rdadan {

// 输出到控制台的 LogAppender
void StdoutAppender::logAppender(std::shared_ptr<Logger> logger,
                                 LogLevel::Level level,
                                 std::shared_ptr<LogEvent> event) {
    // 如果满足log等级，按照格式将日志输出到 stdout
    if (level >= m_level) {
        std::cout << m_formatter->format(logger, level, event) << std::endl;
    }
}
// 输出到文件的 LogAppender
FileAppender::FileAppender(const std::string filename) : m_filename(filename) {
    reopenFile();
}
void FileAppender::logAppender(std::shared_ptr<Logger> logger,
                               LogLevel::Level level,
                               std::shared_ptr<LogEvent> event) {
    // 按照格式将日志输出到 日志文件
    if (level >= m_level) {
        m_filestream << m_formatter->format(logger, level, event);
    }
}

bool FileAppender::reopenFile() {
    if (m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename, std::ios::app);
    return !!m_filestream;
}

} // namespace rdadan
