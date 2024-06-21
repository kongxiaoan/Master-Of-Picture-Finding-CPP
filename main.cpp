#include <iostream>
#include "core/Master.h"
#include "core/log/Logger.h"


extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
}

#include "core/utils/FileUtils.h"


int testFFmpeg(const char *);

int main() {

    Logger::logInfo("TEST", "Hello, World!");
    std::shared_ptr<FFmpegWrapper> ffmpeg = Master::getInstance().getFFmpegWrapper();
    Master::getInstance().init();
    ffmpeg->initializeFFmpeg();

    FileUtils fileUtils;

    std::string textContent = fileUtils.readTextFile("/Users/mtkj/CLionProjects/master-core/debug/example.txt");
    if (!textContent.empty()) {
        Logger::logInfo("Text content read:\n" + textContent);
    }


    testFFmpeg("/Users/mtkj/CLionProjects/master-core/debug/test1.mp4");
    Master::getInstance().getFFmpegWrapper()->extract_audio("/Users/mtkj/CLionProjects/master-core/debug/test1.mp4",
                                                            "/Users/mtkj/CLionProjects/master-core/debug/test3.m4a");
    Logger::logInfo("程序执行结束");
    return 0;
}

/**
 * 使用 FFmpeg 库来读取并打印输入文件的元数据
 *
 * @param filename 输入文件的路径
 * @return 如果成功返回 0，否则返回负数错误码
 */
int testFFmpeg(const char *filename) {
    AVFormatContext *fmt_ctx = nullptr; // 格式上下文，用于存储文件格式信息
    AVDictionaryEntry *tag = nullptr;   // 字典条目，用于存储元数据条目
    int ret;                         // 返回值，用于存储函数调用的返回结果

    // 检查文件名参数是否为空
    if (!filename) {
        Logger::logInfo("用法: testFFmpeg <输入文件>\n"
                        "这是一个演示如何使用 libavformat 库来读取文件元数据的示例程序。\n"
                        "\n");
        return 1; // 返回 1 表示参数错误
    }

    // 打开输入文件并读取文件头，填充 fmt_ctx
    if ((ret = avformat_open_input(&fmt_ctx, filename, nullptr, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "无法打开输入文件 '%s'\n", filename);
        return ret; // 返回负数表示打开文件失败
    }

    // 检索流信息
    if ((ret = avformat_find_stream_info(fmt_ctx, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "无法找到流信息\n");
        avformat_close_input(&fmt_ctx); // 关闭输入文件并释放 fmt_ctx
        return ret; // 返回负数表示检索流信息失败
    }

    // 遍历并打印文件的元数据
    while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
        printf("%s=%s\n", tag->key, tag->value);

    // 关闭输入文件并释放 fmt_ctx
    avformat_close_input(&fmt_ctx);

    return 0; // 成功返回 0
}