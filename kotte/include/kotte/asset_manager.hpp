#pragma once

#include "kotte/asset_registry.hpp"

#include <raylib.h>
#include <unordered_map>

namespace kotte
{
    // Owns every texture the game draws. A texture is loaded from the
    // registry's path the first time its AssetId is requested and served
    // from the map afterwards, so no image is ever loaded twice.
    //
    // Textures live in the window's GL context: create the manager after
    // the window and destroy it before.
    class AssetManager final {
    public:
        explicit AssetManager(AssetRegistry registry);
        ~AssetManager();

        AssetManager(const AssetManager&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;

        // The returned reference stays valid for the manager's lifetime.
        // Throws std::runtime_error if the image cannot be loaded.
        [[nodiscard]] const Texture2D& texture(AssetId id);

    private:
        AssetRegistry registry_;
        std::unordered_map<AssetId, Texture2D> textures_;
    };
}
