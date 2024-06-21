//
// Created by kpa on 2024/6/21.
//

#include "StdLogger.h"

void StdLogger::logInfo(const std::string &message) {
    std::cout << "[INFO] " << message << std::endl;

}

void StdLogger::logInfo(const std::string &tag, const std::string &message) {
    std::cout << tag + " --> " << "[INFO] " << message << std::endl;

}

void StdLogger::logError(const std::string &message) {
    std::cout << "[ERROR] " << message << std::endl;

}

void StdLogger::logError(const std::string &tag, const std::string &message) {
    std::cout << tag + " --> " << "[ERROR] " << message << std::endl;
}
