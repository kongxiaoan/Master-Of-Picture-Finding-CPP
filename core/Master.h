//
// Created by kpa on 2024/6/21.
//
#ifndef MASTER_H
#define MASTER_H

#include "ffmpeg/FFmpegWrapper.h"

#include <memory>
#include "log/Logger.h"

class Master {
public:
    // 获取单例实例的方法
    // 通过静态局部变量保证实例的唯一性和线程安全性（C++11及以上）
    static Master &getInstance() {
        static Master instance; // 局部静态变量，只会被初始化一次
        return instance;
    }

    // 删除拷贝构造函数
    // 防止使用拷贝构造函数生成新的实例
    Master(const Master &) = delete;

    // 删除赋值运算符
    // 防止使用赋值运算符将一个实例赋值给另一个实例
    Master &operator=(const Master &) = delete;

    void init() {
        Logger::logInfo("开始初始化");
    }

    void destory() {

    }

    std::unique_ptr<FFmpegWrapper> getFFmpegWrapper() {
        return std::make_unique<FFmpegWrapper>();
    }

private:
    // 私有化构造函数
    // 防止在类外部通过 new 或其他方式创建实例
    Master() = default;
};

#endif
