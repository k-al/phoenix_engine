
#include <vector>

// #include <SFML/Window>

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
    
//     static uint64_t action_count = std::static_cast<uint64_t>(Action::count);
    
    virtual void set_default_bindings ();
    
    virtual std::vector<bool> get_actions ();
    virtual iVec2 get_move_dir ();
    virtual iVec2 get_cursor_pos ();
};

class KeyboardInteraction : public Interaction {
    sf::Keyboard::Key key_bindings[uint64_t(Interaction::Action::count)];
    sf::Mouse::Button mouse_bindings[uint64_t(Interaction::Action::count)];
    bool is_mouse[uint64_t(Interaction::Action::count)];
    
    bool get_button (Action bt);
    
    void set_default_bindings ();
    
    std::vector<bool> get_actions ();
    iVec2 get_move_dir ();
    iVec2 get_cursor_pos ();
};
