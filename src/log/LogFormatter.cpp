#include "LogFormatter.h"

#include <bits/types/__FILE.h>

#include <functional>
#include <iostream>
#include <map>
#include <sstream>

#include "LogLevel.h"
#include "Logger.h"

namespace rdadan {

LogFormatter::LogFormatter(const std::string &strFomrat)
    : m_strFormat(strFomrat) {
  init();
}
std::string LogFormatter::format(std::shared_ptr<Logger> logger,
                                 LogLevel::Level level,
                                 std::shared_ptr<LogEvent> event) {
  // std::cout << "format " << std::endl;
  // std::cout << __FILE__ << __LINE__ << m_formatItems.size() << std::endl;
  std::stringstream ss;
  for (auto &item : m_formatItems) {
    item->format(ss, logger, level, event);
  }
  return ss.str();
}
// m 消息
class MessageFormatItem : public LogFormatter::FormatItem {
 public:
  MessageFormatItem(const std::string &str = "") {}
  void format(std::ostream &os, Logger::sptr logger, LogLevel::Level level,
              LogEvent::sptr event) override {
    os << event->getContent();
  }
};
// p 日志级别
class LevelFormatItem : public LogFormatter::FormatItem {
 public:
  LevelFormatItem(const std::string &str = "") {}
  void format(std::ostream &os, Logger::sptr logger, LogLevel::Level level,
              LogEvent::sptr event) override {
    os << LogLevel::ToString(level);
  }
};
// r 日志毫秒数
class ElapseFormatItem : public LogFormatter::FormatItem {
 public:
  ElapseFormatItem(const std::string &str = "") {}
  void format(std::ostream &os, Logger::sptr logger, LogLevel::Level level,
              LogEvent::sptr event) override {
    os << event->getElapse();
  }
};
// c 日志名称
class NameFormatItem : public LogFormatter::FormatItem {
 public:
  NameFormatItem(const std::string &str = "") {}
  void format(std::ostream &os, Logger::sptr logger, LogLevel::Level level,
              LogEvent::sptr event) override {
    os << logger->getName();
  }
};
// t 线程id
class ThreadIdFormatItem : public LogFormatter::FormatItem {
 public:
  ThreadIdFormatItem(const std::string &str = "") {}
  void format(std::ostream &os, Logger::sptr logger, LogLevel::Level level,
              LogEvent::sptr event) override {
    os << event->getThreadID();
  }
};
// F 协程id
class FiberIdFormatItem : public LogFormatter::FormatItem {
 public:
  FiberIdFormatItem(const std::string &str = "") {}
  void format(std::ostream &os, Logger::sptr logger, LogLevel::Level level,
              LogEvent::sptr event) override {
    os << event->getFiberID();
  }
};
// d 时间
class DateTimeFormatItem : public LogFormatter::FormatItem {
 public:
  DateTimeFormatItem(const std::string &format = "%Y-%m-%d %H:%M:%S")
      : m_format(format) {
    if (m_format.empty()) {
      m_format = "%Y-%m-%d %H:%M:%S";
    }
  }

  void format(std::ostream &os, Logger::sptr logger, LogLevel::Level level,
              LogEvent::sptr event) override {
    struct tm tm;
    time_t time = event->getTime();
    localtime_r(&time, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), m_format.c_str(), &tm);
    os << buf;
  }

 private:
  std::string m_format;
};
// f 文件名
class FilenameFormatItem : public LogFormatter::FormatItem {
 public:
  FilenameFormatItem(const std::string &str = "") {}
  void format(std::ostream &os, Logger::sptr logger, LogLevel::Level level,
              LogEvent::sptr event) override {
    os << event->getFile();
  }
};
// l 行号
class LineFormatItem : public LogFormatter::FormatItem {
 public:
  LineFormatItem(const std::string &str = "") {}
  void format(std::ostream &os, Logger::sptr logger, LogLevel::Level level,
              LogEvent::sptr event) override {
    os << event->getLine();
  }
};
// n 换行
class NewLineFormatItem : public LogFormatter::FormatItem {
 public:
  NewLineFormatItem(const std::string &str = "") {}
  void format(std::ostream &os, Logger::sptr logger, LogLevel::Level level,
              LogEvent::sptr event) override {
    os << std::endl;
  }
};

class StringFormatItem : public LogFormatter::FormatItem {
 public:
  StringFormatItem(const std::string &str) : m_string(str) {}
  void format(std::ostream &os, Logger::sptr logger, LogLevel::Level level,
              LogEvent::sptr event) override {
    os << m_string;
  }

 private:
  std::string m_string;
};
// T Tab
class TabFormatItem : public LogFormatter::FormatItem {
 public:
  TabFormatItem(const std::string &str = "") {}
  void format(std::ostream &os, Logger::sptr logger, LogLevel::Level level,
              LogEvent::sptr event) override {
    os << "\t";
  }

 private:
  std::string m_string;
};

//%xxx %xxx{xxx} %%
//"%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
// 2022-07-16 22:01:05	15432	666	[INFO]  [root]	test_log.cpp:21 LOG_INFO
void LogFormatter::init() {
  // str, format, type
  std::vector<std::tuple<std::string, std::string, int>> vec;
  std::string nstr;
  for (size_t i = 0; i < m_strFormat.size(); ++i) {
    if (m_strFormat[i] != '%') {
      nstr.append(1, m_strFormat[i]);
      continue;
    }

    if ((i + 1) < m_strFormat.size()) {
      if (m_strFormat[i + 1] == '%') {
        nstr.append(1, '%');
        continue;
      }
    }

    size_t n = i + 1;
    int fmt_status = 0;
    size_t fmt_begin = 0;

    std::string str;
    std::string fmt;
    while (n < m_strFormat.size()) {
      if (!fmt_status && (!isalpha(m_strFormat[n]) && m_strFormat[n] != '{' &&
                          m_strFormat[n] != '}')) {
        str = m_strFormat.substr(i + 1, n - i - 1);
        break;
      }
      if (fmt_status == 0) {
        if (m_strFormat[n] == '{') {
          str = m_strFormat.substr(i + 1, n - i - 1);
          // std::cout << "*" << str << std::endl;
          fmt_status = 1;  //解析格式
          fmt_begin = n;
          ++n;
          continue;
        }
      } else if (fmt_status == 1) {
        if (m_strFormat[n] == '}') {
          fmt = m_strFormat.substr(fmt_begin + 1, n - fmt_begin - 1);
          // std::cout << "#" << fmt << std::endl;
          fmt_status = 0;
          ++n;
          break;
        }
      }
      ++n;
      if (n == m_strFormat.size()) {
        if (str.empty()) {
          str = m_strFormat.substr(i + 1);
        }
      }
    }

    if (fmt_status == 0) {
      if (!nstr.empty()) {
        vec.push_back(std::make_tuple(nstr, std::string(), 0));
        nstr.clear();
      }
      vec.push_back(std::make_tuple(str, fmt, 1));
      i = n - 1;
    } else if (fmt_status == 1) {
      std::cout << "pattern parse error: " << m_strFormat << " - "
                << m_strFormat.substr(i) << std::endl;
      vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
    }
  }

  if (!nstr.empty()) {
    vec.push_back(std::make_tuple(nstr, "", 0));
  }

  // s_format_items 静态map变量，
  // key: #str,对应的格式标符，
  // val：这个格式标符的shared_ptr
  static std::map<std::string,
                  std::function<FormatItem::sptr(const std::string &str)>>
      s_format_items = {
#define XX(str, C)                                                            \
  {                                                                           \
#str, [](const std::string &fmt) { return FormatItem::sptr(new C(fmt)); } \
  }

          XX(m, MessageFormatItem),  XX(p, LevelFormatItem),
          XX(r, ElapseFormatItem),   XX(c, NameFormatItem),
          XX(t, ThreadIdFormatItem), XX(n, NewLineFormatItem),
          XX(d, DateTimeFormatItem), XX(f, FilenameFormatItem),
          XX(l, LineFormatItem),     XX(T, TabFormatItem),
          XX(F, FiberIdFormatItem),
#undef XX
      };

  for (auto &i : vec) {
    if (std::get<2>(i) == 0) {
      m_formatItems.push_back(
          FormatItem::sptr(new StringFormatItem(std::get<0>(i))));
    } else {
      auto it = s_format_items.find(std::get<0>(i));
      if (it == s_format_items.end()) {
        m_formatItems.push_back(FormatItem::sptr(
            new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
      } else {
        m_formatItems.push_back(it->second(std::get<1>(i)));
      }
    }

    // std::cout << "(" << std::get<0>(i) << ") - (" << std::get<1>(i) << ")
    // - (" << std::get<2>(i) << ")" << std::endl;
  }
  // std::cout << m_formatItems.size() << std::endl;
}

}  // namespace rdadan