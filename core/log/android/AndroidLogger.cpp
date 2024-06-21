//
// Created by kpa on 2024/6/21.
//

#include "AndroidLogger.h"
#ifdef ANDROID
#include <android/log.h>
#define LOG_TAG "NativeLogger"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


void AndroidLogger::logInfo(const std::string &message) {
    __android_log_print(ANDROID_LOG_INFO, "AndroidLogger", "%s", message.c_str());
}

void AndroidLogger::logInfo(const std::string &tag, const std::string &message) {
    __android_log_print(ANDROID_LOG_INFO, tag.c_str(), "%s", message.c_str());
}

void AndroidLogger::logError(const std::string &message) {
    __android_log_print(ANDROID_LOG_ERROR, "AndroidLogger", "%s", message.c_str());

}

void AndroidLogger::logError(const std::string &tag, const std::string &message) {
    __android_log_print(ANDROID_LOG_ERROR, tag.c_str(), "%s", message.c_str());
}
#endif

