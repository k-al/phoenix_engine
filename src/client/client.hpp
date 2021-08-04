
#include <SFML/Graphics.hpp>

class Server;
class Thing;

class Client {
  public:
    Server* servre = nullptr;
    Thing* follow = nullptr;
    
    sf::Window main_window;
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
