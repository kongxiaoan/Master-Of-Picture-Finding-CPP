//
// Created by kpa on 2024/6/21.
//

#ifndef MASTER_CORE_FILEUTILS_H
#define MASTER_CORE_FILEUTILS_H

#include <iostream>
#include <fstream>
#include <vector>
#include "../log/Logger.h"


class FileUtils {
public:
    std::unique_ptr<AbstractLogger> logger = Logger::getLogger();

    FileUtils() = default;

    ~FileUtils() = default;

    std::string readTextFile(const std::string &fileName) const {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            logger->logInfo("Error opening file: " + fileName);
            return "";
        }

        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        file.close();
        return content;
    }

    // 读取二进制文件（例如图片、视频等）
    static std::vector<char> readBinaryFile(const std::string &filename) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return {};
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (!file.read(buffer.data(), size)) {
            std::cerr << "Error reading file: " << filename << std::endl;
            return {};
        }

        file.close();
        return buffer;
    }
};


#endif //MASTER_CORE_FILEUTILS_H
