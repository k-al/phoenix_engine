
// #include <SFML/Graphics.hpp> // included by .hpp
#include <iostream>
#include <thread>

#include "server/chunk.hpp"
#include "objects/visible.hpp"
#include "ui.hpp"
#include "../objects/controllable.hpp"

#include "client.hpp"

Client::Client () {
    this->ui = new UI();
}

Client::~Client () {
    // cleanup
    this->main_window.close();
    delete this->ui;
}

void Client::run () {
    this->main_window.create(sf::VideoMode::getFullscreenModes()[1], "Tarnish"/*, sf::Style::Fullscreen*/);
    
    this->main_window.setFramerateLimit(50);
    
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
    
    std::cout << "Main Thread " << std::hex << std::this_thread::get_id() << " got to draw()\n";
    
    if (Controllable* contr_follow = dynamic_cast<Controllable*>(this->follow)) {
        std::cout << "Main Thread " << std::hex << std::this_thread::get_id() << " ask for active chunks\n";
        this->active_chunks = contr_follow->get_active_chunks(5);
        std::cout << "Main Thread " << std::hex << std::this_thread::get_id() << " got " << this->active_chunks.size() << " active chunks\n";
    }
    
    
    for (Chunk* chunk : this->active_chunks) {
        for (Thing* thing : chunk->objects) {
            if (Visible* visible = dynamic_cast<Visible*>(thing)) {
                this->active_sprites.push_back(&visible->sprite);
            }
        }
    }
    
    this->main_window.clear(sf::Color::Black);  
    
    for (sf::Sprite* sprite : this->active_sprites) {
        std::cout << "Main Thread " << std::hex << std::this_thread::get_id() << " draws " << sprite << "\n";
        this->main_window.draw(*sprite);
    }
    
    this->main_window.display();
}
