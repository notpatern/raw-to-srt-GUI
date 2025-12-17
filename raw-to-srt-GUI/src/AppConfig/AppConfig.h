#pragma once

#include <iostream>
#include "json.hpp"
#include <fstream>
#include <string>
#include <string_view>

namespace ConfigHelper {
struct Config {
    std::string audioDevice;
    std::string videoDevice;
    int videoBitrate;
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
};

bool LoadConfig(std::string_view fileName, Config& config) {
    std::ifstream dataFile(fileName.data());
    nlohmann::json data;

    if (!dataFile.is_open()) {
        std::cerr << "Could not open " << fileName.data() << "\n\r";
        return false;
    }

    dataFile >> data;

    config.audioDevice = data.value("audio_device", "OBS_Audio");
    config.videoDevice = data.value("video_device", "/dev/video0");
    config.videoBitrate = data.value("video_bitrate", 8000000);
    config.outputIP = data.value("output_ip", std::string("127.0.0.1"));
    config.outputPort = data.value("output_port", 9000);
    config.transport = data.value("transport", 4);
    config.gopLength = data.value("gop_length", 2);
    config.performance = data.value("performance", 3);
    config.profile = data.value("profile", 77);
    config.entropyMode = data.value("entropy_mode", 1);
    config.pictureMode = data.value("picture_mode", 1);
    config.bitrateMode = data.value("bitrate_mode", 2);
    config.multicast = data.value("multicast", false);
    return true;
}

void SaveConfig(std::string_view fileName, Config& config) {
    nlohmann::json json{
        {"audio_device", config.audioDevice},
        {"video_device", config.videoBitrate,
        {"video_bitrate", config.outputIP},
        {"output_ip", config.outputPort},
        {"output_port", config.transport},
        {"transport", config.gopLength},
        {"gop_length", config.performance},
        {"performance", config.profile},
        {"profile", config.entropyMode},
        {"picture_mode", config.pictureMode},
        {"bitrate_mode", config.bitrateMode},
        {"multicast", config.multicast}
    }};

    std::ofstream dataFile(fileName.data());
    if (!dataFile.is_open()) {
        std::cerr << "config could not be saved, file did not open\n\r";
    }

    dataFile << json;

    return;
}

};
