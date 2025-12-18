#pragma once

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include "../nlohmann/json.hpp"

#ifdef _WIN32
    #define VIDEO_SOURCE "mfvideosrc"
    #define AUDIO_SOURCE "wasapisrc"
    #define VIDEO_DEVICE_PARAM "device-name="
    #define AUDIO_DEVICE_PARAM "device="
    #define DEFAULT_VIDEO_DEVICE "0"
    #define DEFAULT_AUDIO_DEVICE "default"
#else
    #define VIDEO_SOURCE "v4l2src"
    #define AUDIO_SOURCE "pulsesrc"
    #define VIDEO_DEVICE_PARAM "device="
    #define AUDIO_DEVICE_PARAM "device="
    #define DEFAULT_VIDEO_DEVICE "/dev/video2"
    #define DEFAULT_AUDIO_DEVICE "default"
#endif

namespace RawToSrt {
inline int run(std::string audioDevice, std::string videoDevice, int videoBitrate, std::string outputIP, int outputPort, int transport,
        int gopLength, int performance, int profile, int entropyMode, int pictureMode, int bitrateMode, bool multicast) {

    std::ifstream cfgFile("config.json");
    if (!cfgFile.is_open()) {
        std::cerr << "config.json not in current directory\n";
        return 1;
    }

    nlohmann::json cfg;
    try {
        cfgFile >> cfg;
    } catch (const std::exception& e) {
        std::cerr << "error parsing config.json: " << e.what() << "\n";
        return 1;
    }

    std::string sinkConfig;
    if (transport == 1) {
        if (multicast) {
            sinkConfig = "enwsink transport=1 host=239.255.0.1 port=" + std::to_string(outputPort) +
                        " interface=" + outputIP;
        } else {
            sinkConfig = "enwsink transport=1 host=" + outputIP + " port=" + std::to_string(outputPort);
        }
    } else if (transport == 4) {
        sinkConfig = "enwsink transport=4 port=" + std::to_string(outputPort) +
                     " interface=" + outputIP + " rc-mode=0";
    } else {
        std::cerr << "Invalid transport mode. Use 1 for UDP or 4 for SRT\n";
        return 1;
    }

#ifdef _WIN32
    std::string command =
        "gst-launch-1.0 "
        + std::string(VIDEO_SOURCE) + " " + VIDEO_DEVICE_PARAM + videoDevice + " ! "
        "videoconvert ! "
        "eavcenc profile=" + std::to_string(profile) + " "
        "entropy-mode=" + std::to_string(entropyMode) + " "
        "picture-mode=" + std::to_string(pictureMode) + " "
        "bitrate-mode=" + std::to_string(bitrateMode) + " "
        "bitrate-avg=" + std::to_string(videoBitrate) + " "
        "gop-max-length=" + std::to_string(gopLength) + " "
        "performance=" + std::to_string(performance) + " ! "
        "empegmux name=mux ! "
        + sinkConfig + " "
        + std::string(AUDIO_SOURCE) + " " + AUDIO_DEVICE_PARAM;

    if (audioDevice.find(' ') != std::string::npos) {
        command += "\"" + audioDevice + "\"";
    } else {
        command += audioDevice;
    }

    command += " ! "
        "audioconvert ! audioresample ! "
        "eaacenc bitrate=128000 ! "
        "mux.";
#else
    std::string command =
        "gst-launch-1.0 "
        + std::string(VIDEO_SOURCE) + " " + VIDEO_DEVICE_PARAM + videoDevice + " ! "
        "videoconvert ! "
        "eavcenc profile=" + std::to_string(profile) + " "
        "entropy-mode=" + std::to_string(entropyMode) + " "
        "picture-mode=" + std::to_string(pictureMode) + " "
        "bitrate-mode=" + std::to_string(bitrateMode) + " "
        "bitrate-avg=" + std::to_string(videoBitrate) + " "
        "gop-max-length=" + std::to_string(gopLength) + " "
        "performance=" + std::to_string(performance) + " ! "
        "empegmux name=mux ! "
        + sinkConfig + " "
        + std::string(AUDIO_SOURCE) + " " + AUDIO_DEVICE_PARAM;

    if (audioDevice.find(' ') != std::string::npos || audioDevice.find('"') != std::string::npos) {
        command += "\"" + audioDevice + "\"";
    } else {
        command += audioDevice;
    }

    command += " ! "
        "audioconvert ! audioresample ! "
        "eaacenc bitrate=128000 ! "
        "mux.";
#endif

    std::cout << "=== Stream Configuration ===\n";
#ifdef _WIN32
    std::cout << "Platform: Windows\n";
#else
    std::cout << "Platform: Linux\n";
#endif
    std::cout << "Video Source: " << VIDEO_SOURCE << "\n";
    std::cout << "Audio Source: " << AUDIO_SOURCE << "\n";
    std::cout << "Video Device: " << videoDevice << "\n";
    std::cout << "Audio Device: " << audioDevice << "\n";
    std::cout << "Video Bitrate: " << videoBitrate << " bps\n";
    std::cout << "Transport: " << (transport == 4 ? "SRT" : "UDP") << "\n";
    std::cout << "Output: " << outputIP << ":" << outputPort << "\n";
    std::cout << "GOP Length: " << gopLength << " seconds\n";
    std::cout << "Performance: " << performance << "\n";
    std::cout << "Profile: " << profile << "\n";
    std::cout << "\n=== Running Pipeline ===\n";
    std::cout << command << "\n\n";

    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "\nPipeline completed successfully\n";
    } else {
        std::cerr << "\nPipeline error, exit code: " << result << "\n";
    }

    return result;
}

}

