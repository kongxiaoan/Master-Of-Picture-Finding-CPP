cmake_minimum_required(VERSION 3.26)
project(master_core)

set(CMAKE_CXX_STANDARD 17)

# FFmpeg的安装目录，可以通过命令"brew info ffmpeg"获取
set(FFMPEG_DIR /usr/local/Cellar/ffmpeg/7.0_1)
# 头文件搜索路径
include_directories(${FFMPEG_DIR}/include/)
# 动态链接库或静态链接库的搜索路径
link_directories(${FFMPEG_DIR}/lib/)


add_executable(master_core main.cpp
        core/Master.h
        core/ffmpeg/FFmpegWrapper.h
        core/ffmpeg/FFmpegWrapper.cpp
        core/log/AbstractLogger.h
        core/log/Logger.cpp
        core/log/Logger.h
        core/log/android/AndroidLogger.cpp
        core/log/android/AndroidLogger.h
        core/log/std/StdLogger.cpp
        core/log/std/StdLogger.h
        core/utils/FileUtils.cpp
        core/utils/FileUtils.h
)

target_link_libraries(
        master_core
        swscale swresample avcodec avutil avdevice avfilter avformat
)
