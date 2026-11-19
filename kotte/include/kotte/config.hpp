#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

namespace kotte
{
    struct KeyBindings {
        std::string move_up;
        std::string move_down;
        std::string move_left;
        std::string move_right;
        std::string place_bomb;
        std::string quit;
    };

    struct Config {
        std::int64_t tile_map_width = 0;
        std::int64_t tile_map_height = 0;
        std::int64_t seed = 0;
        KeyBindings input;
    };

    void from_json(const nlohmann::json& json, KeyBindings& bindings);
    void to_json(nlohmann::json& json, const KeyBindings& bindings);

    void from_json(const nlohmann::json& json, Config& config);
    void to_json(nlohmann::json& json, const Config& config);

    Config default_config();

    // Reads config_path, validates it against the schema at schema_path, and
    // returns the parsed Config. If config_path is missing, invalid JSON, or
    // fails schema validation, the (invalid) contents are preserved at
    // config_path + ".old", config_path is overwritten with default_config(),
    // and the defaults are returned so the caller can keep running.
    Config load_config(const std::string& config_path, const std::string& schema_path);
}
