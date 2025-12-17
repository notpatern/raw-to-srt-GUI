#include "../AppConfig/AppConfig.h"
#include "../../vendors/srt-to-ndi/rawtondi.h"

class App {
private:
    ConfigHelper::Config m_config;
    void InitGlfw();
    void InitImgui();

public:
    App();
    ~App();

    void run();
};
