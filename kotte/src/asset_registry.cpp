#include "kotte/asset_registry.hpp"

#include "kotte/ldtk.hpp"

#include <stdexcept>
#include <string>

namespace kotte
{
    constexpr std::string_view crates_layer_identifier = "crates";
    constexpr std::string_view player_identifier = "player";
    constexpr std::string_view enemy_identifier = "enemy";

    // Reads a field that LDtk leaves null until it is set in the editor,
    // naming what is missing instead of failing with a bare type error.
    [[nodiscard]] const nlohmann::json& required_field(
        const nlohmann::json& object,
        std::string_view key,
        std::string_view what){
        const nlohmann::json& field = object.at(key);
        if(field.is_null()){
            throw std::runtime_error{"LDtk project does not set " + std::string{what}};
        }
        return field;
    }

    [[nodiscard]] std::filesystem::path tileset_path(const nlohmann::json& definitions, int tileset_uid){
        const nlohmann::json& tileset = ldtk::find_by_uid(definitions.at("tilesets"), tileset_uid);
        const std::string what = "an image for tileset '" + tileset.at("identifier").get<std::string>() + "'";
        return required_field(tileset, "relPath", what).get<std::string>();
    }

    [[nodiscard]] std::filesystem::path entity_tileset_path(
        const nlohmann::json& definitions,
        std::string_view entity_identifier){
        const nlohmann::json& entity = ldtk::find_by_identifier(
            definitions.at("entities"), "identifier", entity_identifier);
        const std::string what = "a tileset for entity '" + std::string{entity_identifier} + "'";
        return tileset_path(definitions, required_field(entity, "tilesetId", what).get<int>());
    }

    std::string_view to_string(AssetId id) noexcept{
        switch(id){
        case AssetId::floor: return "floor";
        case AssetId::wall: return "wall";
        case AssetId::crate: return "crate";
        case AssetId::player: return "player";
        case AssetId::enemy: return "enemy";
        }
        return "unknown";
    }

    AssetRegistry AssetRegistry::from_ldtk(
        const nlohmann::json& project,
        const std::filesystem::path& project_directory){
        const nlohmann::json& definitions = project.at("defs");

        const nlohmann::json& crates_layer = ldtk::find_by_identifier(
            definitions.at("layers"), "identifier", crates_layer_identifier);
        const int crate_tileset_uid = required_field(
            crates_layer, "tilesetDefUid", "a tileset for layer 'crates'").get<int>();

        const std::filesystem::path floor_path = required_field(
            ldtk::first_level(project), "bgRelPath", "a background image for the level").get<std::string>();

        AssetRegistry registry;
        registry.add(AssetId::floor, project_directory / floor_path);
        registry.add(AssetId::wall, project_directory / wall_relative_path);
        registry.add(AssetId::crate, project_directory / tileset_path(definitions, crate_tileset_uid));
        registry.add(AssetId::player, project_directory / entity_tileset_path(definitions, player_identifier));
        registry.add(AssetId::enemy, project_directory / entity_tileset_path(definitions, enemy_identifier));
        return registry;
    }

    void AssetRegistry::add(AssetId id, const std::filesystem::path& path){
        paths_.insert_or_assign(id, path.lexically_normal());
    }

    const std::filesystem::path& AssetRegistry::path(AssetId id) const{
        return paths_.at(id);
    }
}
