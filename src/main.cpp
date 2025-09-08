#include "core/application.h"
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    auto app = std::make_unique<Application>();
    
    if (!app->initialize("Final Thrust")) {
        std::cerr << "Failed to initialize application\n";
        return 1;
    }
    
    app->run();
    app->shutdown();
    
    return 0;
}