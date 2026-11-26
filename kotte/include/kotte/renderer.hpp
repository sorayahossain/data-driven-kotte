#pragma once

#include <cstddef>
#include <cstdint>
#include <raylib.h>
#include <vector>

namespace kotte
{
    enum class RenderLayer : std::uint8_t {
        ground,
        effect,
        world
    };

    struct RenderCommand final {
        Rectangle screen_bounds{};
        Color color{};
        RenderLayer layer = RenderLayer::world;
        float depth = 0.0f;
        float roundness = 0.0f;
        // An invalid (default-constructed) texture means "no art yet": the
        // command falls back to a plain rounded rectangle in `color`.
        Texture2D texture{};
        // The part of `texture` to draw, e.g. one frame of a spritesheet. An
        // empty (default) rectangle draws the whole texture; a negative
        // width/height flips the source, as in DrawTexturePro.
        Rectangle source{};
    };

    class Renderer final {
    public:
        void clear() noexcept;
        void submit(RenderCommand command);
        void sort();
        void execute() const;
        [[nodiscard]] std::size_t command_count() const noexcept;

    private:
        std::vector<RenderCommand> commands_;
    };
}
