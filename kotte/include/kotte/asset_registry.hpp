#pragma once

#include <cstdint>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string_view>
#include <unordered_map>

namespace kotte
{
    // The images the game draws, named by what they show rather than by
    // file name.
    enum class AssetId : std::uint8_t {
        floor,
        wall,
        crate,
        player,
        enemy
    };

    [[nodiscard]] std::string_view to_string(AssetId id) noexcept;

    // Knows which file holds each AssetId. The paths come from the LDtk
    // project, so the game draws with the same images the level editor
    // shows:
    // - player / enemy: the tileset their entity definition is drawn with;
    // - crate: the tileset of the "crates" layer's auto-tiles;
    // - floor: the first level's background image;
    // - wall: LDtk has no wall layer, so this one is a fixed file next to
    //   the project (wall_relative_path).
    class AssetRegistry final {
    public:
        static constexpr std::string_view wall_relative_path = "raw/bombman/tile_env4_wall.png";

        // LDtk stores paths relative to the .ldtk file, so they are resolved
        // against project_directory (the folder that holds it). Throws
        // std::runtime_error if any of the four images is not set in LDtk.
        [[nodiscard]] static AssetRegistry from_ldtk(
            const nlohmann::json& project,
            const std::filesystem::path& project_directory);

        // Throws std::out_of_range if id has no registered path.
        [[nodiscard]] const std::filesystem::path& path(AssetId id) const;

    private:
        // Registers path (normalized) as the file that holds id.
        void add(AssetId id, const std::filesystem::path& path);

        std::unordered_map<AssetId, std::filesystem::path> paths_;
    };
}
