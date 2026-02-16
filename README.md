# Raw to SRT — GUI

An ImGui-based graphical interface for [raw-to-srt](https://github.com/notpatern/raw-to-srt). Configure and control your SRT stream without touching the config file directly. Linux only.

> **First time setting up?** If you're installing for the first time or are experiencing issues with GStreamer or audio devices, please refer to the [raw-to-srt documentation](https://github.com/notpatern/raw-to-srt) before continuing.

# Installation
```shell
$ git clone https://github.com/notpatern/raw-to-srt-GUI.git
$ cd raw-to-srt-GUI
$ sudo chmod +x compile.sh
$ ./compile.sh
```

# Prerequisites

- [OBS](https://github.com/obsproject/obs-studio)
- [pulseaudio](https://www.freedesktop.org/wiki/Software/PulseAudio/)
- [pipewire](https://pipewire.org/)
- [Elecard Gstreamer SDK](https://elecard.com/products/sdks/gstreamer-codec-sdk)
- [v4l2-utils](https://github.com/gjasny/v4l-utils)
- [nlohmann/json](https://github.com/nlohmann/json)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [GLFW3](https://www.glfw.org/)
- [Vulkan](https://www.vulkan.org/)

# Running
```shell
$ ./raw-to-srt-GUI
```

# Interface

The GUI exposes all configuration options available in `config.json`. Changes are applied by clicking **Save Config**, which writes directly to the config file.

| Field | Values | Description |
|-------|--------|-------------|
| Audio Device | string | PulseAudio/PipeWire sink name to capture audio from |
| Video Device | string | V4L2 video device path (e.g. `/dev/video0`) |
| Video Bitrate | integer | Video bitrate in kbps |
| Video Framerate | integer | Target framerate |
| Output IP | string | Destination IP address |
| Output Port | integer | Destination port |
| Transport | `UDP`, `SRT` | Streaming transport protocol |
| GOP Length | integer | GOP length in frames |
| GOP Max B-Count | `0`–`3` | Maximum number of B-frames per GOP |
| GOP Max Length | `1`–`500` | Maximum GOP length in frames |
| Performance | `0`: Custom (default), `1`: Highest Quality, `2`: High Quality, `3`: Optimal, `4`: Fast Speed, `5`: Fastest Speed | Encoder speed/quality preset |
| Profile | `Baseline`, `Constrained Baseline`, `Main`, `High`, `High 4:2:2` | H.264 encoding profile |
| Level | `1` – `6.2`, `auto` | H.264 encoding level |
| Entropy Mode | `CAVLC`, `CABAC` | Entropy coding mode |
| Picture Mode | `Interlaced`, `Progressive` | Frame encoding mode |
| Bitrate Mode | `CQP`, `VBR`, `CBR` | Bitrate control mode |
| VBV Size | `1024`–`288000000` | VBV buffer size in bits |
| AFD Code | `Auto`, `Box 16x9 Top`, `Box 14x9 Top`, `Box 16x9 Center`, `Full Frame`, `4x3 Center`, `16x9 Center`, `14x9 Center`, `4x3 SNP 14x9 Center`, `16x9 SNP 14x9 Center`, `16x9 SNP 4x3 Center` | Active Format Description code |
| AR Auto | boolean | Automatically detect aspect ratio |
| AR Mode | `PAR`, `SAR` | Aspect ratio mode |
| B-Frame Mode | `All Disposable`, `All Reference`, `Hierarchical` | B-frame reference mode |
| Chroma Format | `Grayscale`, `4:2:0`, `4:2:2` | Chroma subsampling format |
| Field Order | `Top Field First`, `Bottom Field First`, `Auto` | Field order for interlaced content |
| Video Format | `Component`, `PAL`, `NTSC`, `SECAM`, `MAC`, `Unspecified` | Video format standard |
| Multicast | boolean | Enable multicast (UDP only) |
