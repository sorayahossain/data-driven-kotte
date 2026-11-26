#include "kotte/ldtk.hpp"

#include <fstream>
#include <stdexcept>
#include <string>

namespace kotte::ldtk
{
    nlohmann::json load_project(const std::filesystem::path& project_path){
        std::ifstream project_file{project_path};
        if(!project_file.is_open()){
            throw std::runtime_error{"Could not open " + project_path.string()};
        }

        try{
            return nlohmann::json::parse(project_file);
        } catch(const nlohmann::json::parse_error& error){
            throw std::runtime_error{project_path.string() + " is not valid JSON: " + error.what()};
        }
    }

    const nlohmann::json& find_by_identifier(
        const nlohmann::json& array,
        std::string_view key,
        std::string_view identifier){
        for(const nlohmann::json& element : array){
            if(element.at(key).get_ref<const std::string&>() == identifier){
                return element;
            }
        }

        throw std::runtime_error{
            "LDtk project has no element with " + std::string{key} + " '" + std::string{identifier} + "'"};
    }

    const nlohmann::json& find_by_uid(const nlohmann::json& array, int uid){
        for(const nlohmann::json& element : array){
            if(element.at("uid").get<int>() == uid){
                return element;
            }
        }

        throw std::runtime_error{"LDtk project has no element with uid " + std::to_string(uid)};
    }

    const nlohmann::json& first_level(const nlohmann::json& project){
        const nlohmann::json& levels = project.at("levels");
        if(levels.empty()){
            throw std::runtime_error{"LDtk project has no levels"};
        }

        const nlohmann::json& level = levels.front();
        if(level.at("layerInstances").is_null()){
            throw std::runtime_error{
                "LDtk level '" + level.at("identifier").get<std::string>()
                    + "' is saved in a separate file; disable \"Save levels to separate files\" in LDtk"};
        }

        return level;
    }
}
