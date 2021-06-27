
#include <SFML/Graphics.hpp> // included by .hpp


#include "client.hpp"

Client::~Client () {
    // cleanup
}

void Client::run () {
    this->main_window.create(sf::VideoMode::getFullscreenModes()[0], "Tarnish");
    
    
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
