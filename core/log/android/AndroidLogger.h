//
// Created by kpa on 2024/6/21.
//

#ifndef MASTER_CORE_ANDROIDLOGGER_H
#define MASTER_CORE_ANDROIDLOGGER_H
#include <iostream>
#include <sstream>
#include "../AbstractLogger.h"

class AndroidLogger : public AbstractLogger {
public:
    void logInfo(const std::string &message) override;

    void logInfo(const std::string &tag, const std::string &message) override;

    void logError(const std::string &message) override;

    void logError(const std::string &tag, const std::string &message) override;
};


#endif //MASTER_CORE_ANDROIDLOGGER_H
