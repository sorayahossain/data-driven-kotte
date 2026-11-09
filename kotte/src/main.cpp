#include "kotte/application.hpp"

#include <cstdint>
#include <exception>
#include <fstream>
#include <stdexcept>
#include <string>

int main([[maybe_unused]] int argc, [[maybe_unused]]  char** argv){
    try{
        std::ifstream config_file{"config/config.txt"};
        if(!config_file.is_open()){
            throw std::runtime_error{"Could not open config/config.txt"};
        }

        std::string width_line;
        std::string height_line;
        std::string seed_line;
        if(!std::getline(config_file, width_line)
            || !std::getline(config_file, height_line)
            || !std::getline(config_file, seed_line)){
            throw std::runtime_error{"config/config.txt must have width, height and seed on separate lines"};
        }

        const int tile_map_width = std::stoi(width_line);
        const int tile_map_height = std::stoi(height_line);
        const std::uint64_t seed = std::stoull(seed_line);

        kotte::Application app{1280, 720, "Kotte", tile_map_width, tile_map_height, seed};
        app.run();
    } catch(const std::exception& error){
        TraceLog(LOG_ERROR, "%s", error.what());
        return 1;
    }
    return 0;
}
