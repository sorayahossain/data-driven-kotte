#pragma once

#include "kotte/config.hpp"

#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace kotte
{
    // Maps a key name such as "W" or "SPACE" (one of the values accepted by
    // config_schema.json's key_name definition) to the matching raylib
    // KEY_* constant. Throws std::runtime_error for an unrecognized name.
    [[nodiscard]] int key_from_name(const std::string& name);

    enum class Action {
        quit,
        place_bomb
    };

    enum class Direction {
        move
    };

    class Input final{
    public:
        Input() = default;

        void bind(Action action, std::vector<int> keys);
        void bind(
            Direction direction,
            std::vector<int> up,
            std::vector<int> down,
            std::vector<int> left,
            std::vector<int> right);

        [[nodiscard]] bool down(Action action) const noexcept;
        [[nodiscard]] bool pressed(Action action) const noexcept;
        [[nodiscard]] Vector2 get(Direction direction) const noexcept;

    private:
        struct DirectionBinding final {
            std::vector<int> up;
            std::vector<int> down;
            std::vector<int> left;
            std::vector<int> right;
        };

        [[nodiscard]] static bool any_down(const std::vector<int>& keys) noexcept;
        [[nodiscard]] static bool any_pressed(const std::vector<int>& keys) noexcept;

        std::unordered_map<Action, std::vector<int>> actions_;
        std::unordered_map<Direction, DirectionBinding> directions_;
    };
}
