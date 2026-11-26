#include "kotte/application.hpp"
#include "kotte/asset_registry.hpp"
#include "kotte/config.hpp"
#include "kotte/ldtk.hpp"
#include "kotte/level.hpp"

#include <exception>
#include <filesystem>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){
    try{
        const kotte::Config config = kotte::load_config("config/config.json", "config/config_schema.json");

        // The LDtk project is the single source for both the level layout
        // and the images it is drawn with.
        const std::filesystem::path ldtk_path{"assets/kotte.ldtk"};
        const nlohmann::json ldtk_project = kotte::ldtk::load_project(ldtk_path);
        const kotte::Level level = kotte::Level::from_ldtk(ldtk_project);
        const kotte::AssetRegistry asset_registry = kotte::AssetRegistry::from_ldtk(
            ldtk_project, ldtk_path.parent_path());

        kotte::Application app{1280, 720, "Kotte", config, level, asset_registry};
        app.run();
    } catch(const std::exception& error){
        TraceLog(LOG_ERROR, "%s", error.what());
        return 1;
    }
    return 0;
}
