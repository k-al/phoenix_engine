
// #include <SFML/Graphics.hpp> // included by .hpp

#include "server/chunk.hpp"
#include "objects/visible.hpp"

#include "client.hpp"

Client::Client () {}

Client::~Client () {
    // cleanup
}

void Client::run () {
    this->main_window.create(sf::VideoMode::getFullscreenModes()[1], "Tarnish"/*, sf::Style::Fullscreen*/);
    
    
    sf::Event event;
    
    while (this->main_window.isOpen()) {
        while (this->main_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                //? popup with promt
                this->main_window.close();
            }
        }
        
        
        
    }
}

void Client::draw () {
    for (Chunk* chunk : this->active_chunks) {
        for (Thing* thing : chunk->objects) {
            if (Visible* visible = dynamic_cast<Visible*>(thing)) {
                this->active_sprites.push_back(&visible->sprite);
            }
        }
    }
}
