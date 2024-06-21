//
// Created by kpa on 2024/6/21.
//

#ifndef MASTER_CORE_FFMPEGWRAPPER_H
#define MASTER_CORE_FFMPEGWRAPPER_H
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

}
#include <iostream>

class FFmpegWrapper {
public:

    const char *TAG = "FFMPEGG-TAG";

    FFmpegWrapper() = default;


    ~FFmpegWrapper() = default;

    void initializeFFmpeg();

    void cleanupFFmpeg();


    // 视频转码
    int transcode(const char *input_file, const char *output_file);

    // 提取音频
    int extract_audio(const char *input_file, const char *output_file);

    // 视频剪辑
    int clip_video(const char *input_file, const char *output_file, int start_time, int duration);

    // 视频合并
    int merge_videos(const char *input_files[], int file_count, const char *output_file);

private:
    void log_error(int err);
};


#endif //MASTER_CORE_FFMPEGWRAPPER_H
