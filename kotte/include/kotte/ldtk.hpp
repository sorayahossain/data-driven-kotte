#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>
#include <string_view>

// Small helpers for reading an LDtk project (https://ldtk.io/json). Level
// and AssetRegistry each pick the parts they need out of the same project.
namespace kotte::ldtk
{
    // Reads and parses the .ldtk file. Throws std::runtime_error if it cannot
    // be opened or is not valid JSON.
    [[nodiscard]] nlohmann::json load_project(const std::filesystem::path& project_path);

    // Returns the element of `array` whose `key` field equals `identifier`.
    // LDtk definitions name themselves with "identifier", placed instances
    // with "__identifier". Throws std::runtime_error if there is none.
    [[nodiscard]] const nlohmann::json& find_by_identifier(
        const nlohmann::json& array,
        std::string_view key,
        std::string_view identifier);

    // Returns the element of `array` whose "uid" equals `uid`. Throws
    // std::runtime_error if there is none.
    [[nodiscard]] const nlohmann::json& find_by_uid(const nlohmann::json& array, int uid);

    // Returns the project's first level. Throws std::runtime_error if the
    // project has no level, or stores its levels in separate files
    // ("Save levels to separate files"), which kotte does not read.
    [[nodiscard]] const nlohmann::json& first_level(const nlohmann::json& project);
}
