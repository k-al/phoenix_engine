// header guard
#ifndef TEXTURE_MANAGER_HEADER
#define TEXTURE_MANAGER_HEADER



#include "../defs.hpp"

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>
#include <mutex>

class TextureManager {
public:
    
    struct TextureObject {
        sf::Texture tex;
        uint32 use_counter
        std::mutex mtx;
    }
    
    static std::unordered_map<std::string, TextureObject> textures;
    
    static get (const std::string&);
    
    static drop (const std::string&);
}

#endif
