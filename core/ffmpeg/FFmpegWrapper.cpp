//
// Created by kpa on 2024/6/21.
//

#include "FFmpegWrapper.h"
#include "iostream"
#include "../log/Logger.h"

#define ERROR_STR_SIZE 1024

void FFmpegWrapper::initializeFFmpeg() {
    std::cout << "ffmpeg init" << std::endl;
}

void FFmpegWrapper::cleanupFFmpeg() {

}

int FFmpegWrapper::transcode(const char *input_file, const char *output_file) {
    return 0;
}

int FFmpegWrapper::extract_audio(const char *input_file, const char *output_file) {
    AVFormatContext *input_fmt_ctx = nullptr;
    AVFormatContext *output_fmt_ctx = nullptr;
    //支持各种各样的输出文件格式，MP4，FLV，3GP等等
    const AVOutputFormat *output_fmt = nullptr;
    //输入流
    AVStream *in_stream = nullptr;
    //输出流
    AVStream *out_stream = nullptr;
    //存储压缩数据
    AVPacket packet;
    char errors[1024];
    AVPacket pkt;


    //要拷贝的流
    int audio_stream_index = -1;
    int ret;

    //打开输入文件
    if ((ret = avformat_open_input(&input_fmt_ctx, input_file, nullptr, nullptr)) < 0) {
        log_error(ret);
        return ret;
    }

    if (input_fmt_ctx->nb_streams < 2) {
        //流数小于2，说明这个文件音频、视频流这两条都不能保证，输入文件有错误
        av_log(nullptr, AV_LOG_ERROR, "输入文件错误，流不足2条\n");
        avformat_close_input(&input_fmt_ctx);
        return -1;
    }

    //拿到文件中音频流
    in_stream = input_fmt_ctx->streams[1];
    //参数信息
    AVCodecParameters *in_codecpar = in_stream->codecpar;
    //找到最好的音频流
    audio_stream_index = av_find_best_stream(input_fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (audio_stream_index < 0) {
        av_log(nullptr, AV_LOG_DEBUG, "寻找最好音频流失败，请检查输入文件！\n");
        avformat_close_input(&input_fmt_ctx);
        return AVERROR(EINVAL);
    }

    // 获取音频流对象
    AVStream *audio_stream = input_fmt_ctx->streams[audio_stream_index];
    if (!audio_stream) {
        avformat_close_input(&input_fmt_ctx);
        av_log(nullptr, AV_LOG_ERROR, "无法获取音频流！\n");
        return AVERROR(EINVAL);
    }

// 输出音频流的格式信息
    AVCodecParameters *codec_params = audio_stream->codecpar;
    const AVCodecDescriptor *codec_desc = avcodec_descriptor_get(codec_params->codec_id);
    if (codec_desc) {
        av_log(nullptr, AV_LOG_INFO, "音频流信息:\n");
        av_log(nullptr, AV_LOG_INFO, "编解码器名称: %s\n", codec_desc->name);
        av_log(nullptr, AV_LOG_INFO, "采样率: %d Hz\n", codec_params->sample_rate);
        // 其他信息如有需要可以继续添加输出
    } else {
        avformat_close_input(&input_fmt_ctx);
        av_log(nullptr, AV_LOG_ERROR, "未知编解码器！\n");
        return AVERROR_INVALIDDATA;
    }

    // 输出上下文
    output_fmt_ctx = avformat_alloc_context();
    const char *format_name = nullptr; // 可以根据文件名自动猜测
    //根据目标文件名生成最适合的输出容器
    output_fmt = av_guess_format(format_name, output_file, nullptr);
    if (!output_fmt) {
        av_log(nullptr, AV_LOG_DEBUG, "根据目标生成输出容器失败！\n");
        avformat_close_input(&input_fmt_ctx);
        avformat_free_context(output_fmt_ctx);
        return -1;
    }
    Logger::logInfo(output_fmt->extensions);
    output_fmt_ctx->oformat = output_fmt;
    //新建输出流
    out_stream = avformat_new_stream(output_fmt_ctx, nullptr);
    if (!out_stream) {
        av_log(nullptr, AV_LOG_DEBUG, "创建输出流失败！\n");
        avformat_close_input(&input_fmt_ctx);
        avformat_free_context(output_fmt_ctx);
        return -1;
    }

    // 将参数信息拷贝到输出流中，我们只是抽取音频流，并不做音频处理，所以这里只是Copy
    if ((ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar)) < 0) {
        av_strerror(ret, errors, ERROR_STR_SIZE);
        av_log(NULL, AV_LOG_ERROR, "拷贝编码参数失败！, %d(%s)\n",
               ret, errors);
        avformat_close_input(&input_fmt_ctx);
        avformat_free_context(output_fmt_ctx);
        return -1;
    }

    out_stream->codecpar->codec_tag = 0;

    //初始化AVIOContext,文件操作由它完成
    if ((ret = avio_open(&output_fmt_ctx->pb, output_file, AVIO_FLAG_WRITE)) < 0) {
        av_strerror(ret, errors, 1024);
        av_log(NULL, AV_LOG_DEBUG, "文件打开失败 %s, %d(%s)\n",
               output_file,
               ret,
               errors);
        avformat_close_input(&input_fmt_ctx);
        avformat_free_context(output_fmt_ctx);
        return -1;
    }


    av_dump_format(output_fmt_ctx, 0, output_file, 1);


    //初始化 AVPacket， 我们从文件中读出的数据会暂存在其中
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;


    // 写头部信息
    if (avformat_write_header(output_fmt_ctx, NULL) < 0) {
        av_log(NULL, AV_LOG_DEBUG, "写入头部信息失败！");
        avformat_close_input(&input_fmt_ctx);
        avformat_free_context(output_fmt_ctx);
        return -1;
    }

    //每读出一帧数据
    while (av_read_frame(input_fmt_ctx, &pkt) >= 0) {
        if (pkt.stream_index == audio_stream_index) {
            pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base,
                                       (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base,
                                       (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));

            pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
            pkt.pos = -1;
            pkt.stream_index = 0;
            //将包写到输出媒体文件
            av_interleaved_write_frame(output_fmt_ctx, &pkt);
            //减少引用计数，避免内存泄漏
            av_packet_unref(&pkt);
        }
    }

    //写尾部信息
    av_write_trailer(output_fmt_ctx);

    //最后别忘了释放内存
    avformat_close_input(&input_fmt_ctx);
    avio_close(output_fmt_ctx->pb);
    avformat_free_context(output_fmt_ctx);
    return 0;
}

int FFmpegWrapper::clip_video(const char *input_file, const char *output_file, int start_time, int duration) {
    return 0;
}

int FFmpegWrapper::merge_videos(const char **input_files, int file_count, const char *output_file) {
    return 0;
}

void FFmpegWrapper::log_error(int err) {
    char errbuf[128];
    av_strerror(err, errbuf, sizeof(errbuf));
    Logger::logError(TAG, errbuf);
}
