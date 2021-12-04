
// #include <SFML/Graphics.hpp> // included by .hpp

#include "server/chunk.hpp"
#include "objects/visible.hpp"
#include "ui.hpp"
#include "../objects/controllable.hpp"

#include "client.hpp"

class Controllable;

Client::Client () {}

Client::~Client () {
    // cleanup
    this->main_window.close();
}

void Client::run () {
    this->main_window.create(sf::VideoMode::getFullscreenModes()[1], "Tarnish"/*, sf::Style::Fullscreen*/);
    
    
    sf::Event event;
    
    while (this->main_window.isOpen()) {
        while (this->main_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                //? popup with promt
                return;
            }
        }
        
        
        this->draw();
    }
}

void Client::change_follow (Thing* obj) {
    this->follow = obj;
    
    this->ui->change_follow(dynamic_cast<Controllable*>(obj)); // call it with nullptr if it is not Controllable
}

void Client::draw () {
    for (Chunk* chunk : this->active_chunks) {
        for (Thing* thing : chunk->objects) {
            if (Visible* visible = dynamic_cast<Visible*>(thing)) {
                this->active_sprites.push_back(&visible->sprite);
            }
        }
    }
    
    for (sf::Sprite* sprite : this->active_sprites) {
        this->main_window.draw(*sprite);
    }
}
