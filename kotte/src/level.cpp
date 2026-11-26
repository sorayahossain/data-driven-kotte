#include "kotte/level.hpp"

#include "kotte/ldtk.hpp"

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>

namespace kotte
{
    constexpr std::string_view entities_layer_identifier = "entities";
    constexpr std::string_view crates_layer_identifier = "crates";
    constexpr std::string_view player_identifier = "player";
    constexpr std::string_view enemy_identifier = "enemy";

    [[nodiscard]] std::string describe(TilePosition position){
        return "(" + std::to_string(position.column) + ", " + std::to_string(position.row) + ")";
    }

    void require_inside(const Level& level, TilePosition position, std::string_view what){
        const bool inside = position.column >= 0 && position.column < level.width
            && position.row >= 0 && position.row < level.height;
        if(!inside){
            throw std::runtime_error{
                std::string{what} + " at tile " + describe(position)
                    + " is outside the level's " + std::to_string(level.width) + "x"
                    + std::to_string(level.height) + " complete tiles"};
        }
    }

    void require_not_on_crate(const Level& level, TilePosition position, std::string_view what){
        if(std::ranges::find(level.crates, position) != level.crates.end()){
            throw std::runtime_error{std::string{what} + " at tile " + describe(position) + " starts on a crate"};
        }
    }

    Level Level::from_ldtk(const nlohmann::json& project){
        const nlohmann::json& ldtk_level = ldtk::first_level(project);
        const nlohmann::json& layers = ldtk_level.at("layerInstances");
        const nlohmann::json& entities_layer = ldtk::find_by_identifier(
            layers, "__identifier", entities_layer_identifier);
        const nlohmann::json& crates_layer = ldtk::find_by_identifier(
            layers, "__identifier", crates_layer_identifier);

        // Both layers must share one grid, or an entity's cell would not be
        // the same tile as the crate cell with the same coordinates.
        const int grid_size = crates_layer.at("__gridSize").get<int>();
        if(grid_size <= 0 || entities_layer.at("__gridSize").get<int>() != grid_size){
            throw std::runtime_error{"LDtk layers 'entities' and 'crates' must use the same grid size"};
        }

        Level level;
        level.width = ldtk_level.at("pxWid").get<int>() / grid_size;
        level.height = ldtk_level.at("pxHei").get<int>() / grid_size;
        if(level.width <= 0 || level.height <= 0){
            throw std::runtime_error{"LDtk level is smaller than one grid cell"};
        }

        // The CSV covers every cell LDtk knows about, partial ones included,
        // row by row with __cWid cells per row.
        const int csv_columns = crates_layer.at("__cWid").get<int>();
        const nlohmann::json& crate_cells = crates_layer.at("intGridCsv");
        for(std::size_t index = 0; index < crate_cells.size(); ++index){
            if(crate_cells[index].get<int>() == 0){
                continue;
            }

            const int cell = static_cast<int>(index);
            const TilePosition position{cell % csv_columns, cell / csv_columns};
            require_inside(level, position, "Crate");
            level.crates.push_back(position);
        }

        bool player_found = false;
        for(const nlohmann::json& instance : entities_layer.at("entityInstances")){
            const std::string& identifier = instance.at("__identifier").get_ref<const std::string&>();
            const nlohmann::json& cell = instance.at("__grid");
            const TilePosition position{cell.at(0).get<int>(), cell.at(1).get<int>()};

            if(identifier == player_identifier){
                if(player_found){
                    throw std::runtime_error{"LDtk level has more than one 'player' entity"};
                }
                require_inside(level, position, "Player");
                level.player_spawn = position;
                player_found = true;
            } else if(identifier == enemy_identifier){
                require_inside(level, position, "Enemy");
                level.enemy_spawns.push_back(position);
            }
        }

        if(!player_found){
            throw std::runtime_error{"LDtk level has no 'player' entity"};
        }

        // A mover spawned inside a solid crate could never move again.
        require_not_on_crate(level, level.player_spawn, "Player");
        for(const TilePosition enemy_spawn : level.enemy_spawns){
            require_not_on_crate(level, enemy_spawn, "Enemy");
        }

        return level;
    }
}
