//
// Created by kpa on 2024/6/21.
//

#ifndef MASTER_CORE_LOGGER_H
#define MASTER_CORE_LOGGER_H

#include <memory>
#include "AbstractLogger.h"
#include "std/StdLogger.h"
#include "android/AndroidLogger.h"

class Logger {
public:

    static void logInfo(const std::string &message) {
        getLogger()->logInfo(message);
    }

    static void logInfo(const std::string &tag, const std::string &message) {
        getLogger()->logInfo(tag, message);
    }

    static void logError(const std::string &message) {
        getLogger()->logError(message);
    }

    static void logError(const std::string &tag, const std::string &message) {
        getLogger()->logError(tag, message);
    }


    static std::unique_ptr<AbstractLogger> getLogger() {
#ifdef ANDROID
        return std::make_unique<AndroidLogger>();
#else
        return std::make_unique<StdLogger>();
#endif
    }

};


#endif //MASTER_CORE_LOGGER_H
