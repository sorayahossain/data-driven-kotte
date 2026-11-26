#include "kotte/asset_manager.hpp"

#include <stdexcept>
#include <string>
#include <utility>

namespace kotte
{
    AssetManager::AssetManager(AssetRegistry registry)
        : registry_{std::move(registry)}{
    }

    AssetManager::~AssetManager(){
        for(const auto& entry : textures_){
            UnloadTexture(entry.second);
        }
    }

    const Texture2D& AssetManager::texture(AssetId id){
        if(const auto cached = textures_.find(id); cached != textures_.end()){
            return cached->second;
        }

        const std::filesystem::path& path = registry_.path(id);
        const Texture2D loaded = LoadTexture(path.string().c_str());
        if(!IsTextureValid(loaded)){
            throw std::runtime_error{
                "Could not load the " + std::string{to_string(id)} + " texture from " + path.string()};
        }

        return textures_.emplace(id, loaded).first->second;
    }
}
