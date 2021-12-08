// header guard
#ifndef CLIENT_HEADER
#define CLIENT_HEADER

#include "interaction.hpp"

#include <SFML/Graphics.hpp>



class Server;
class Thing;
class UI;

class Client {
  public:
    Server* server = nullptr;
    UI* ui = nullptr;
    Interaction* interaction = nullptr;
    Thing* follow = nullptr;
    
    
    
    sf::RenderWindow main_window;
    sf::View main_view;
    
    std::vector<sf::Sprite*> active_sprites;
    
    std::vector<Chunk*> active_chunks;
    
    
    Client ();
    
    ~Client ();
    
    
    // creates a Window and manage it
    // the calling thread should be the main thread (iOS compatibility) and only returns if the Window is closed
    void run ();
    
    void change_follow (Thing*);
    
    void draw ();
    
};


#endif // header guard
