#pragma once

#include <cstdlib>
#include <functional>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <atomic>
#include <vector>
#include "../nlohmann/json.hpp"
#include "../TinyProcessLib/process.hpp"

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

class Runner {
public:
    using OutputCallback = std::function<void(const char*, size_t)>;

    std::unique_ptr<TinyProcessLib::Process> process;
    std::atomic<int> exit_status{-1};

    Runner() = default;
    ~Runner() { stop(); }

    bool start(std::string audioDevice1, std::string audioDevice2, std::string audioDevice3, std::string audioDevice4,
               std::string audioDevice5, std::string audioDevice6, std::string audioDevice7, std::string audioDevice8,
               std::string videoDevice, int videoBitrate, int videoFramerate, std::string outputIP, int outputPort, int transport,
               int gopLength, int performance, int profile, int entropyMode, int pictureMode, int bitrateMode, bool multicast, 
               int afdCode, int arAuto, int bFramesMode, int chromaFormat, int fieldOrder, int gopMaxBcount, int gopMaxLength, 
               int vbvSize, int videoFormat, int level);

    void stop();

    bool is_running() const { return process && exit_status.load() == -1; }

    int wait();

    void setStdoutCallback(OutputCallback callback) {stdoutCallback = callback;}
    void setStderrCallback(OutputCallback callback) {stderrCallback = callback;}

private:
    OutputCallback stdoutCallback;
    OutputCallback stderrCallback;
};

inline bool Runner::start(std::string audioDevice1, std::string audioDevice2, std::string audioDevice3, std::string audioDevice4,
                          std::string audioDevice5, std::string audioDevice6, std::string audioDevice7, std::string audioDevice8,
                          std::string videoDevice, int videoBitrate, int videoFramerate, std::string outputIP, int outputPort, int transport,
                          int gopLength, int performance, int profile, int entropyMode, int pictureMode, int bitrateMode, bool multicast,
                          int afdCode, int arAuto, int bFramesMode, int chromaFormat, int fieldOrder, int gopMaxBcount, int gopMaxLength,
                          int vbvSize, int videoFormat, int level) {

    stop();

    std::vector<std::string> audioDevices;
    if (!audioDevice1.empty()) audioDevices.push_back(audioDevice1);
    if (!audioDevice2.empty()) audioDevices.push_back(audioDevice2);
    if (!audioDevice3.empty()) audioDevices.push_back(audioDevice3);
    if (!audioDevice4.empty()) audioDevices.push_back(audioDevice4);
    if (!audioDevice5.empty()) audioDevices.push_back(audioDevice5);
    if (!audioDevice6.empty()) audioDevices.push_back(audioDevice6);
    if (!audioDevice7.empty()) audioDevices.push_back(audioDevice7);
    if (!audioDevice8.empty()) audioDevices.push_back(audioDevice8);

    if (audioDevices.empty()) {
        std::cerr << "At least one audio device must be specified\n";
        return false;
    }

    if (audioDevices.size() > 8) {
        std::cerr << "Maximum 8 audio devices supported\n";
        return false;
    }

    std::ifstream cfgFile("config.json");
    if (!cfgFile.is_open()) {
        std::cerr << "config.json not in current directory\n";
        return false;
    }

    nlohmann::json cfg;
    try {
        cfgFile >> cfg;
    } catch (const std::exception& e) {
        std::cerr << "error parsing config.json: " << e.what() << "\n";
        return false;
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
        return false;
    }

    std::string command = "gst-launch-1.0 ";
    
    command += std::string(VIDEO_SOURCE) + " " + VIDEO_DEVICE_PARAM + videoDevice + " ! "
        "videoconvert ! "
        "eavcenc profile=" + std::to_string(profile) + " "
        "entropy-mode=" + std::to_string(entropyMode) + " "
        "picture-mode=" + std::to_string(pictureMode) + " "
        "bitrate-mode=" + std::to_string(bitrateMode) + " "
        "bitrate-avg=" + std::to_string(videoBitrate) + " "
        "gop-max-length=" + std::to_string(gopLength) + " "
        "fps=" + std::to_string(videoFramerate) + " "
        "afd-code=" + std::to_string(afdCode) + " "
        "ar-auto=" + std::to_string(arAuto) + " "
        "b-frames-mode=" + std::to_string(bFramesMode) + " "
        "chroma-format=" + std::to_string(chromaFormat) + " "
        "field-order=" + std::to_string(fieldOrder) + " "
        "gop-max-bcount=" + std::to_string(gopMaxBcount) + " "
        "gop-max-length=" + std::to_string(gopMaxLength) + " "
        "vbv-size=" + std::to_string(vbvSize) + " "
        "video-format=" + std::to_string(videoFormat) + " "
        "level=" + std::to_string(level) + " "
        "performance=" + std::to_string(performance) + " ! "
        "empegmux name=mux ! " + sinkConfig + " ";

    for (size_t i = 0; i < audioDevices.size(); ++i) {
        const std::string& audioDevice = audioDevices[i];
        
        command += std::string(AUDIO_SOURCE) + " " + AUDIO_DEVICE_PARAM;
        
        if (audioDevice.find(' ') != std::string::npos || audioDevice.find('"') != std::string::npos) {
            command += "\"" + audioDevice + "\"";
        } else {
            command += audioDevice;
        }
        
        command += " ! "
            "audioconvert ! audioresample ! "
            "audio/x-raw,channels=1 ! "  // Force mono
            "eaacenc bitrate=128000 ! "
            "mux. ";
    }

    std::cout << "=== Stream Configuration ===\n";
#ifdef _WIN32
    std::cout << "Platform: Windows\n";
#else
    std::cout << "Platform: Linux\n";
#endif
    std::cout << "Video Source: " << VIDEO_SOURCE << "\n";
    std::cout << "Audio Source: " << AUDIO_SOURCE << "\n";
    std::cout << "Video Device: " << videoDevice << "\n";
    std::cout << "Audio Devices (" << audioDevices.size() << "):\n";
    for (size_t i = 0; i < audioDevices.size(); ++i) {
        std::cout << "  [" << (i + 1) << "] " << audioDevices[i] << "\n";
    }
    std::cout << "Video Bitrate: " << videoBitrate << " bps\n";
    std::cout << "Video Framerate: " << videoFramerate << "\n";
    std::cout << "Transport: " << (transport == 4 ? "SRT" : "UDP") << "\n";
    std::cout << "Output: " << outputIP << ":" << outputPort << "\n";
    std::cout << "GOP Length: " << gopLength << " seconds\n";
    std::cout << "Performance: " << performance << "\n";
    std::cout << "Profile: " << profile << "\n";
    std::cout << "\n=== Running Pipeline ===\n";
    std::cout << command << "\n\n";

    process = std::make_unique<TinyProcessLib::Process>(command, "",
                                                        [this](const char* bytes, size_t n) {
                                                        if (stdoutCallback) {
                                                        stdoutCallback(bytes, n);
                                                        }
                                                        },
                                                        [this](const char* bytes, size_t n) {
                                                        if (stderrCallback) {
                                                        stderrCallback(bytes, n);
                                                        }
                                                        });

    exit_status = -1;
    return true;
}

inline void Runner::stop() {
    if (process) {
        process->kill(true);
        process.reset();
        exit_status = 0;
        std::cout << "remember me, for I gave sweat and blood until freedom came.\n\r";
    }
}

inline int Runner::wait() {
    if (process) {
        exit_status = process->get_exit_status();
    }
    return exit_status.load();
}
}
