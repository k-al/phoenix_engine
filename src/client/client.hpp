
// #include <SFML/Graphics.hpp>

class Server;
class Thing;

class Client {
    
    Server* servre = nullptr;
    Thing* follow = nullptr;
    
    Client () {}
    
    ~Client ();
    
    
    // creates a Window and manage it
    void run ();
    
    void change_follow (Thing*);
    
};
