
#ifndef CONTOLLABLE_HEADER
#define CONTOLLABLE_HEADER

#include <vector>

class Controllable {
  public:
    std::function<void(Thing* that, std::vector<bool> actions, iVec2 move_dir, iVec2 cursor_pos)> action;
};


#endif // CONTOLLABLE_HEADER
