#include "kotte/character_sprite.hpp"

#include <array>
#include <cstddef>

namespace kotte
{
    constexpr int standing_column = 0;

    // Columns 2-4 are one stride. Bouncing back through the middle pose
    // (2, 3, 4, 3, ...) keeps the legs alternating instead of snapping
    // from the last pose straight back to the first.
    constexpr std::array walk_cycle_columns{2, 3, 4, 3};
    constexpr float walk_frame_seconds = 0.12f;

    [[nodiscard]] int row_for(CardinalDirection facing) noexcept{
        switch(facing){
        case CardinalDirection::down: return 0;
        case CardinalDirection::up: return 1;
        case CardinalDirection::left: return 2;
        case CardinalDirection::right: return 3;
        }
        return 0;
    }

    [[nodiscard]] int column_for(float animation_time) noexcept{
        if(animation_time <= 0.0f){
            return standing_column;
        }

        const auto step = static_cast<std::size_t>(animation_time / walk_frame_seconds);
        return walk_cycle_columns[step % walk_cycle_columns.size()];
    }

    CharacterSprite::CharacterSprite(const Texture2D& texture) noexcept
        : texture_{texture}{
    }

    const Texture2D& CharacterSprite::texture() const noexcept{
        return texture_;
    }

    Vector2 CharacterSprite::frame_size() const noexcept{
        return {
            static_cast<float>(texture_.width) / static_cast<float>(column_count),
            static_cast<float>(texture_.height) / static_cast<float>(row_count)
        };
    }

    Rectangle CharacterSprite::frame(CardinalDirection facing, float animation_time) const noexcept{
        const Vector2 size = frame_size();
        return {
            static_cast<float>(column_for(animation_time)) * size.x,
            static_cast<float>(row_for(facing)) * size.y,
            size.x,
            size.y
        };
    }
}
