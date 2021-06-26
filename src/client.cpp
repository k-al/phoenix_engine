
#include <SFML/Graphics.hpp> // included by .hpp


#include "client.hpp"

Client::~Client () {
    // cleanup
}

Client::run () {
    this->window.create(sf::VideoMode::getFullscreenModes()[0], "Tarnish");
    
    while (this->window.isOpen()) {
        
    }
}
