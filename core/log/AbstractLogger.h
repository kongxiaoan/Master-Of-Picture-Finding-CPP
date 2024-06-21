//
// Created by kpa on 2024/6/21.
//

#ifndef MASTER_CORE_ABSTRACTLOGGER_H
#define MASTER_CORE_ABSTRACTLOGGER_H

#include <iostream>
#include <memory> // for std::unique_ptr


class AbstractLogger {
public:
    virtual ~AbstractLogger() {}

    virtual void logInfo(const std::string &message) = 0;

    virtual void logInfo(const std::string &tag, const std::string &message) = 0;

    virtual void logError(const std::string &message) = 0;

    virtual void logError(const std::string &tag, const std::string &message) = 0;
};


#endif //MASTER_CORE_ABSTRACTLOGGER_H
