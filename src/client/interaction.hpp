// header guard
#ifndef INTERACTION_HEADER
#define INTERACTION_HEADER

#include <vector>

#include <SFML/Window.hpp>

#include "../ivec.hpp"

class Interaction {
  public:
    enum class Action {
        up = 0,
        down,
        right,
        left,
        sprint,
        crouch,
        dash,
        inventar,
        use,
        reload,
        hotbar_1,
        hotbar_2,
        hotbar_3,
        item_1,
        item_2,
        item_3,
        item_4,
        item_5,
        prim_attack,
        sec_attack,
        tert_attack,
        count  // Must stay last. Used for array size
    };
    
    virtual ~Interaction () = 0;
    
    virtual void set_default_bindings () = 0;
    
    virtual std::vector<bool> get_actions () = 0;
    virtual iVec2 get_move_dir () = 0;
    virtual iVec2 get_cursor_pos () = 0;
};

class KeyboardInteraction : public Interaction {
  public:
    sf::Keyboard::Key key_bindings[uint64_t(Interaction::Action::count)];
    sf::Mouse::Button mouse_bindings[uint64_t(Interaction::Action::count)];
    bool is_mouse[uint64_t(Interaction::Action::count)];
    
    KeyboardInteraction ();
    ~KeyboardInteraction ();
    
    bool get_button (Action bt);
    
    void set_default_bindings ();
    
    std::vector<bool> get_actions ();
    iVec2 get_move_dir ();
    iVec2 get_cursor_pos ();
};

#endif // header guard
