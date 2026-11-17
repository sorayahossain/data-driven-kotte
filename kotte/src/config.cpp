#include "kotte/config.hpp"

#include <nlohmann/json-schema.hpp>
#include <raylib.h>

#include <fstream>
#include <iterator>
#include <stdexcept>

namespace kotte
{
    void from_json(const nlohmann::json& json, KeyBindings& bindings){
        // load input from json
    }

    void to_json(nlohmann::json& json, const KeyBindings& bindings){
        // dump config to json
    }

    void from_json(const nlohmann::json& json, Config& config){
        json.at("tile_map_width").get_to(config.tile_map_width);
        json.at("tile_map_height").get_to(config.tile_map_height);
        json.at("seed").get_to(config.seed);
    }

    void to_json(nlohmann::json& json, const Config& config){
        json = nlohmann::json{
            {"tile_map_width", config.tile_map_width},
            {"tile_map_height", config.tile_map_height},
            {"seed", config.seed},
        };
    }

    Config default_config(){
        return Config{
            .tile_map_width = 17,
            .tile_map_height = 13,
            .seed = 323453473861,
            .input = KeyBindings{
                .move_up = "W",
                .move_down = "S",
                .move_left = "A",
                .move_right = "D",
                .place_bomb = "SPACE",
                .quit = "Q"
            }
        };
    }

    namespace {
        void write_config(const std::string& config_path, const Config& config){
            std::ofstream config_file{config_path, std::ios::trunc};
            config_file << nlohmann::json(config).dump(4);
        }
    }

    Config load_config(const std::string& config_path, const std::string& schema_path){
        std::ifstream schema_file{schema_path};
        if(!schema_file.is_open()){
            throw std::runtime_error{"Could not open " + schema_path};
        }

        nlohmann::json_schema::json_validator validator;
        validator.set_root_schema(nlohmann::json::parse(schema_file));

        std::ifstream config_file{config_path};
        if(!config_file.is_open()){
            throw std::runtime_error{"Could not open " + config_path};
        }

        const std::string raw_content{
            std::istreambuf_iterator<char>{config_file},
            std::istreambuf_iterator<char>{}};
        config_file.close();

        nlohmann::json config_json = nlohmann::json::parse(raw_content);
        validator.validate(config_json);
        return config_json.get<Config>();
    }
}
