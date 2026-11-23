#pragma once

#include <string>
#include <vector>

namespace kotte
{
    // A parsed level layout read from a plain-text grid file. 'P' marks the
    // player's start tile, 'E' marks an enemy start, '#' marks a crate, and
    // any other character is empty floor.
    struct Level final {
        std::vector<std::string> rows;
        int player_column = 0;
        int player_row = 0;

        [[nodiscard]] int width() const noexcept;
        [[nodiscard]] int height() const noexcept;
        [[nodiscard]] char at(int x, int y) const noexcept;
    };

    // Reads level_path as a grid of characters. Throws std::runtime_error if
    // the file cannot be opened, its row count does not equal expected_height,
    // any row's length does not equal expected_width, or it does not contain
    // exactly one 'P'.
    [[nodiscard]] Level load_level(
        const std::string& level_path,
        int expected_width,
        int expected_height);
}
