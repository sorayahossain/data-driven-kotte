#include "kotte/application.hpp"
#include "kotte/config.hpp"
#include "kotte/ldtk.hpp"
#include "kotte/level.hpp"

#include <exception>
#include <filesystem>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){
    try{
        const kotte::Config config = kotte::load_config("config/config.json", "config/config_schema.json");

        // The level layout comes from the LDtk project.
        const std::filesystem::path ldtk_path{"assets/kotte.ldtk"};
        const nlohmann::json ldtk_project = kotte::ldtk::load_project(ldtk_path);
        const kotte::Level level = kotte::Level::from_ldtk(ldtk_project);

        kotte::Application app{1280, 720, "Kotte", config, level};
        app.run();
    } catch(const std::exception& error){
        TraceLog(LOG_ERROR, "%s", error.what());
        return 1;
    }
    return 0;
}
