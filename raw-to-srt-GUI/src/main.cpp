#include "App/App.h"

int main() {
    App* app = new App();   //ptr so i can just call delete
    app->run();             //and free all the process' memory at once
    delete app;
    std::terminate();
}
