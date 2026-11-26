#pragma once

#include "kotte/entity.hpp"

#include <raylib.h>

namespace kotte
{
    // Picks the part of a character spritesheet to draw. A sheet is a grid
    // of equally sized frames, 5 columns by 4 rows:
    //   column 0     standing still
    //   column 1     kicking a bomb (not used yet)
    //   columns 2-4  the walk cycle
    //   rows         facing down, up, left, right (in that order)
    // The frame size is derived from the texture, so a resized sheet keeps
    // working as long as it keeps that grid.
    class CharacterSprite final {
    public:
        static constexpr int column_count = 5;
        static constexpr int row_count = 4;

        explicit CharacterSprite(const Texture2D& texture) noexcept;

        [[nodiscard]] const Texture2D& texture() const noexcept;
        [[nodiscard]] Vector2 frame_size() const noexcept;

        // The source rectangle for a character facing `facing` that has been
        // walking for `animation_time` seconds; 0 (or less) means standing
        // still.
        [[nodiscard]] Rectangle frame(CardinalDirection facing, float animation_time) const noexcept;

    private:
        Texture2D texture_;
    };
}
