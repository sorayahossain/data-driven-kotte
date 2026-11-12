#include "kotte/application.hpp"

#include <nlohmann/json-schema.hpp>
#include <nlohmann/json.hpp>

#include <cstdint>
#include <exception>
#include <fstream>
#include <stdexcept>

int main([[maybe_unused]] int argc, [[maybe_unused]]  char** argv){
    try{
        std::ifstream schema_file{"config/config_schema.json"};
        if(!schema_file.is_open()){
            throw std::runtime_error{"Could not open config/config_schema.json"};
        }

        nlohmann::json_schema::json_validator validator;
        validator.set_root_schema(nlohmann::json::parse(schema_file));

        std::ifstream config_file{"config/config.json"};
        if(!config_file.is_open()){
            throw std::runtime_error{"Could not open config/config.json"};
        }

        const nlohmann::json config = nlohmann::json::parse(config_file);
        validator.validate(config);

        const int tile_map_width = config.at("tile_map_width").get<int>();
        const int tile_map_height = config.at("tile_map_height").get<int>();
        const std::uint64_t seed = config.at("seed").get<std::uint64_t>();

        kotte::Application app{1280, 720, "Kotte", tile_map_width, tile_map_height, seed};
        app.run();
    } catch(const std::exception& error){
        TraceLog(LOG_ERROR, "%s", error.what());
        return 1;
    }
    return 0;
}
