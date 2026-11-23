#include "kotte/level.hpp"

#include <fstream>
#include <stdexcept>

namespace kotte
{
    int Level::width() const noexcept{
        return rows.empty() ? 0 : static_cast<int>(rows.front().size());
    }

    int Level::height() const noexcept{
        return static_cast<int>(rows.size());
    }

    char Level::at(int x, int y) const noexcept{
        return rows[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
    }

    Level load_level(
        const std::string& level_path,
        int expected_width,
        int expected_height){
        std::ifstream level_file{level_path};
        if(!level_file.is_open()){
            throw std::runtime_error{"Could not open " + level_path};
        }

        Level level;
        bool player_found = false;
        std::string line;
        while(std::getline(level_file, line)){
            if(!line.empty() && line.back() == '\r'){
                line.pop_back(); // tolerate CRLF line endings
            }

            if(static_cast<int>(line.size()) != expected_width){
                throw std::runtime_error{
                    level_path + " row " + std::to_string(level.rows.size())
                        + " has " + std::to_string(line.size())
                        + " columns, expected " + std::to_string(expected_width)};
            }

            for(std::size_t x = 0; x < line.size(); ++x){
                if(line[x] != 'P'){
                    continue;
                }

                if(player_found){
                    throw std::runtime_error{level_path + " has more than one 'P' start tile"};
                }

                player_found = true;
                level.player_column = static_cast<int>(x);
                level.player_row = static_cast<int>(level.rows.size());
            }

            level.rows.push_back(std::move(line));
        }

        if(static_cast<int>(level.rows.size()) != expected_height){
            throw std::runtime_error{
                level_path + " has " + std::to_string(level.rows.size())
                    + " rows, expected " + std::to_string(expected_height)};
        }

        if(!player_found){
            throw std::runtime_error{level_path + " has no 'P' start tile"};
        }

        return level;
    }
}
