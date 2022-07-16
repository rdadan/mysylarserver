cmake_minimum_required(VERSION 2.8)
project(mylog)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -g -Wall -Wno-deprecated -Werror -Wno-unused-function")
add_definitions("-Wall -g")

include_directories(.)


# 查找source目录下的所有*.cpp,*.c源文件,并将文件列表保存到 DIR_LIB_SRCS 变量
file(GLOB_RECURSE LIB_SRC "src/*.cpp" "src/log/*.cpp")	#遍历获取src 路径下所有的*.cpp列表

# set(LIB_SRC
#     src/log/Log.cpp
#     src/log/LogAppender.cpp
#     src/log/LogEvent.cpp
#     src/log/LogFormatter.cpp
#     # src/log/LogLevel.cpp
#     src/util.cpp
#     )

message("stc:" {LIB_SRC})
#将DIR_LIB_SRCS中文件列表以静态库的方式加入工程
# add_library( ${PROJECT_NAME} STATIC  ${LIB_SRC}) 
#将DIR_LIB_SRCS 中文件列表以静态库的方式加入工程
add_library(mylog SHARED ${LIB_SRC}) 




include_directories(/usr/include/) # 添加yanl的头文件路径 
link_directories(/usr/lib) # 添加yaml的连接库
find_library(YAMLCPP yaml-cpp)
message("***" ${YAMLCPP})

# 编译 test_log.cpp
add_executable(test_log test/test_log.cpp)
add_dependencies(test_log mylog)
target_link_libraries(test_log mylog ${YAMLCPP})

# 编译 test_config.cpp
# add_executable(test_config tests/test_config.cpp)
# add_dependencies(test_config rdadan)
# target_link_libraries(test_config rdadan ${YAMLCPP})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
