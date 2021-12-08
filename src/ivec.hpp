// header guard
#ifndef IVEC2_HEADER
#define IVEC2_HEADER

#include <SFML/Graphics.hpp>

//# inline all funktions
struct iVec2 {
    int64_t x;
    int64_t y;
    
    // constructors
    iVec2 ();
    
    iVec2 (int64_t x, int64_t y);
    
    // operators
    bool operator== (const iVec2 &rhs) const;
    
    bool operator!= (const iVec2 &rhs) const;
    
    iVec2 operator* (int64_t mul) const;
    
    void operator*= (int64_t mul);
    
    iVec2 operator/ (int64_t mul) const;
    
    void operator/= (int64_t mul);
    
    iVec2 operator+ (const iVec2 add) const;
    
    void operator+= (const iVec2 add);
    
    iVec2 operator- (const iVec2 sub) const;
    
    void operator-= (const iVec2 sub);
    
    iVec2 operator<< (const unsigned int shift) const;
    
    void operator<<= (const unsigned int shift);
    
    iVec2 operator>> (const unsigned int shift) const;
    
    void operator>>= (const unsigned int shift);
    
    
    iVec2 (const sf::Vector2i set);
    
    void operator= (const sf::Vector2i set);
    
    sf::Vector2f as_vec2f () const;
    
    sf::Vector2f as_vec2f (const float scale) const;
    
    double distance () const;
    
    double distance (const iVec2 to) const;
    
    uint64_t fast_distance (const iVec2 to) const;
};

#endif // IVEC2_HEADER
