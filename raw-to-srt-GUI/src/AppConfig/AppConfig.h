#pragma once

#include <iostream>
#include "json.hpp"
#include <fstream>
#include <string>
#include <string_view>

namespace ConfigHelper {
struct Config {
    std::string audioDevice1;
    std::string audioDevice2;
    std::string audioDevice3;
    std::string audioDevice4;
    std::string audioDevice5;
    std::string audioDevice6;
    std::string audioDevice7;
    std::string audioDevice8;
    std::string videoDevice;
    int videoBitrate;
    int videoFramerate;
    std::string outputIP;
    int outputPort;
    int transport;
    int gopLength;
    int performance;
    int profile;
    int entropyMode;
    int pictureMode;
    int bitrateMode;
    bool multicast;
    bool arAuto;
    int arMode;
    int bframeMode;
    int fieldOrder;
    int gopMaxBcount;
    int gopMaxLength;
    int vbvSize;
    int videoFormat;
    int afdCode;
    int level;
    int chromaFormat;
};

inline bool LoadConfig(std::string_view fileName, Config& config) {
    std::ifstream dataFile(fileName.data());
    nlohmann::json data;

    if (!dataFile.is_open()) {
        std::cerr << "Could not open " << fileName.data() << "\n\r";
        return false;
    }

    dataFile >> data;

    config.audioDevice1 = data.value("audio_device1", "OBS_Audio");
    config.audioDevice2 = data.value("audio_device2", "OBS_Audio");
    config.audioDevice3 = data.value("audio_device3", "OBS_Audio");
    config.audioDevice4 = data.value("audio_device4", "OBS_Audio");
    config.audioDevice5 = data.value("audio_device5", "OBS_Audio");
    config.audioDevice6 = data.value("audio_device6", "OBS_Audio");
    config.audioDevice7 = data.value("audio_device7", "OBS_Audio");
    config.audioDevice8 = data.value("audio_device8", "OBS_Audio");
    config.videoDevice = data.value("video_device", "/dev/video0");
    config.videoBitrate = data.value("video_bitrate", 8000000);
    config.videoFramerate = data.value("video_framerate", 25);
    config.outputIP = data.value("output_ip", std::string("127.0.0.1"));
    config.outputPort = data.value("output_port", 9000);
    config.transport = data.value("transport", 4);
    config.gopLength = data.value("gop_length", 2);
    config.performance = data.value("performance", 0);
    config.profile = data.value("profile", 100);
    config.entropyMode = data.value("entropy_mode", 1);
    config.pictureMode = data.value("picture_mode", 1);
    config.bitrateMode = data.value("bitrate_mode", 2);
    config.multicast = data.value("multicast", false);
    config.arAuto = data.value("ar_auto", true);
    config.arMode = data.value("ar_mode", 1);
    config.bframeMode = data.value("b_frame_mode", 0);
    config.fieldOrder = data.value("field_order", 2);
    config.gopMaxBcount = data.value("gop_max_b_count", 0);
    config.gopMaxLength = data.value("gop_max_length", 250);
    config.vbvSize = data.value("vbv_size", 1200000);
    config.videoFormat = data.value("video_format", 5);
    config.afdCode = data.value("afd_code", 0);
    config.level = data.value("level", 100);
    config.chromaFormat = data.value("chromaFormat", 1);
    return true;
}

inline void SaveConfig(std::string_view fileName, Config& config) {
    nlohmann::json json{
        {"audio_device1", config.audioDevice1},
        {"audio_device2", config.audioDevice2},
        {"audio_device3", config.audioDevice3},
        {"audio_device4", config.audioDevice4},
        {"audio_device5", config.audioDevice5},
        {"audio_device6", config.audioDevice6},
        {"audio_device7", config.audioDevice7},
        {"audio_device8", config.audioDevice8},
        {"video_device", config.videoDevice},
        {"video_bitrate", config.videoBitrate},
        {"video_framerate", config.videoFramerate},
        {"output_ip", config.outputIP},
        {"output_port", config.outputPort},
        {"transport", config.transport},
        {"gop_length", config.gopLength},
        {"performance", config.performance},
        {"profile", config.profile},
        {"entropy_mode", config.entropyMode},
        {"picture_mode", config.pictureMode},
        {"bitrate_mode", config.bitrateMode},
        {"multicast", config.multicast},
        {"ar_auto", config.arAuto},
        {"ar_mode", config.arMode},
        {"b_frame_mode", config.bframeMode},
        {"field_order", config.fieldOrder},
        {"gop_max_b_count", config.gopMaxBcount},
        {"gop_max_length", config.gopMaxLength},
        {"vbv_size", config.vbvSize},
        {"video_format", config.videoFormat},
        {"afd_code", config.afdCode},
        {"level", config.level},
        {"chroma_format", config.chromaFormat}
    };

    std::ofstream dataFile(fileName.data());
    if (!dataFile.is_open()) {
        std::cerr << "config could not be saved, file did not open\n\r";
        return;
    }
    dataFile << json.dump(4);
    return;
}

};
