#pragma once

#include <nlohmann/json.hpp>
#include <vector>

namespace kotte
{
    // A tile coordinate: columns grow rightwards, rows grow downwards.
    struct TilePosition final {
        int column = 0;
        int row = 0;

        [[nodiscard]] bool operator==(const TilePosition&) const noexcept = default;
    };

    // Where everything starts in a level, in tile coordinates.
    struct Level final {
        int width = 0;
        int height = 0;
        TilePosition player_spawn;
        std::vector<TilePosition> enemy_spawns;
        std::vector<TilePosition> crates;

        // Reads the project's first level: one "player" and any number of
        // "enemy" instances on its "entities" layer, and a crate on every
        // non-zero cell of its "crates" IntGrid layer. Other entity kinds
        // are ignored.
        //
        // LDtk grids are square (50x50) but kotte's tiles are 50x45, so
        // pixel coordinates (px, __worldX/Y) do not map onto kotte's world.
        // Only cell coordinates are read: an entity's __grid and a crate's
        // index in the IntGrid CSV. width/height count only complete cells;
        // LDtk rounds a partial cell at the level's right/bottom edge up
        // (a 1306px-wide level has 27 columns), and that sliver is not
        // playable.
        //
        // Throws std::runtime_error if the layers use different grid sizes,
        // there is not exactly one player, anything sits outside the
        // complete cells, or a player/enemy starts on a crate.
        [[nodiscard]] static Level from_ldtk(const nlohmann::json& project);
    };
}
