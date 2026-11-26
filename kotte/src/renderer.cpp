#include "kotte/renderer.hpp"

#include <algorithm>

namespace kotte
{
    void Renderer::clear() noexcept{
        commands_.clear();
    }

    void Renderer::submit(RenderCommand command){
        commands_.push_back(command);
    }

    void Renderer::sort(){
        std::ranges::sort(commands_, [](const RenderCommand& left, const RenderCommand& right){
            if(left.layer != right.layer){
                return left.layer < right.layer;
            }
            return left.depth < right.depth;
        });
    }

    void Renderer::execute() const{
        for(const RenderCommand& command : commands_){
            if(IsTextureValid(command.texture)){
                // Only the default {0,0,0,0} means "no source". A negative
                // width/height is raylib's way to flip a source, so it must
                // pass through untouched.
                const bool whole_texture = command.source.width == 0.0f && command.source.height == 0.0f;
                const Rectangle source = whole_texture
                    ? Rectangle{
                        0.0f, 0.0f,
                        static_cast<float>(command.texture.width),
                        static_cast<float>(command.texture.height)}
                    : command.source;
                DrawTexturePro(command.texture, source, command.screen_bounds, {0.0f, 0.0f}, 0.0f, WHITE);
                continue;
            }

            DrawRectangleRounded(command.screen_bounds, command.roundness, 6, command.color);
        }
    }

    std::size_t Renderer::command_count() const noexcept{
        return commands_.size();
    }
}
