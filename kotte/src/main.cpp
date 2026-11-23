#include "kotte/application.hpp"
#include "kotte/config.hpp"
#include "kotte/level.hpp"

#include <exception>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){
    try{
        const kotte::Config config = kotte::load_config("config/config.json", "config/config_schema.json");
        const kotte::Level level = kotte::load_level(
            "config/level.txt",
            static_cast<int>(config.tile_map_width),
            static_cast<int>(config.tile_map_height));

        kotte::Application app{1280, 720, "Kotte", config, level};
        app.run();
    } catch(const std::exception& error){
        TraceLog(LOG_ERROR, "%s", error.what());
        return 1;
    }
    return 0;
}
