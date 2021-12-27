
// #include <SFML/Graphics.hpp> // included by .hpp

#include "server/chunk.hpp"
#include "objects/visible.hpp"
#include "ui.hpp"
#include "../objects/controllable.hpp"

#include "client.hpp"

#include <iostream> // just for debugging
#include <thread>

Client::Client () {
    this->ui = new UI();
    this->interaction = new KeyboardInteraction();
    this->interaction->set_default_bindings();
}

Client::~Client () {
    // cleanup
    this->main_window.close();
    delete this->ui;
    delete this->interaction;
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
        
        this->ui->process_input(this->interaction->get_actions(), this->interaction->get_move_dir(), this->interaction->get_cursor_pos());
        
        this->draw();
    }
}

void Client::change_follow (Thing* obj) {
    this->follow = obj;
    
    this->ui->change_follow(dynamic_cast<Controllable*>(obj)); // call it with nullptr if it is not Controllable
}

void Client::draw () {
    
    this->active_sprites.clear();
    
    if (Controllable* contr_follow = dynamic_cast<Controllable*>(this->follow)) {
//         std::cout << "Main Thread " << std::hex << std::this_thread::get_id() << " ask for active chunks\n";
        if (!contr_follow->get_active_chunks(this->active_chunks)) {
            // contr_follow->get_active_chunks(this->active_chunks, 2);
        }
//         std::cout << "draw() got " << this->active_chunks.size() << " active chunks\n";
    }
    
    
    for (Chunk* chunk : this->active_chunks) {
//         std::cout << std::hex << "chunk(" << chunk << ") got " << chunk->objects.size() << " objects\n";
        for (Thing* thing : chunk->objects) {
            if (Visible* visible = dynamic_cast<Visible*>(thing)) {
                this->active_sprites.push_back(&visible->sprite);
            }
        }
//         std::cout << "new number of Visibles: " << this->active_sprites.size() << " objects\n";
    }
    
    this->main_window.clear(sf::Color::Black);  
    
    for (sf::Sprite* sprite : this->active_sprites) {
//         std::cout << "draw " << sprite << "\n";
        this->main_window.draw(*sprite);
    }
    
    this->main_window.display();
}
