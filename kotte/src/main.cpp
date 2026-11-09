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

        std::string seed_line;
        if(!std::getline(config_file, seed_line)){
            throw std::runtime_error{"config/config.txt must have the seed on its first line"};
        }

        const std::uint64_t seed = std::stoull(seed_line);

        kotte::Application app{1280, 720, "Kotte", seed};
        app.run();
    } catch(const std::exception& error){
        TraceLog(LOG_ERROR, "%s", error.what());
        return 1;
    }
    return 0;
}
