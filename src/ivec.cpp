
#include <cmath>
// #include <SFML/Graphics.hpp> // eingebunden durch .hpp

#include "ivec.hpp"

// BEGIN struct iVec2:

// constructors
iVec2::iVec2 () {
    this->x = 0;
    this->y = 0;
}

iVec2::iVec2 (int32 x, int32 y) {
    this->x = x;
    this->y = y;
}

// operators
bool iVec2::operator== (const iVec2 &rhs) const {
    return this->x == rhs.x &&
           this->y == rhs.y;
}

bool iVec2::operator!= (const iVec2 &rhs) const {
    return this->x != rhs.x ||
           this->y != rhs.y;
}

iVec2 iVec2::operator* (int32 mul) const {
    return iVec2(this->x * mul, this->y * mul);
}

void iVec2::operator*= (int32 mul) {
    this->x *= mul;
    this->y *= mul;
}

iVec2 iVec2::operator/ (int32 div) const {
    return iVec2(this->x / div, this->y / div);
}

void iVec2::operator/= (int32 div) {
    this->x /= div;
    this->y /= div;
}

iVec2 iVec2::operator+ (const iVec2 add) const {
    return iVec2(this->x + add.x, this->y + add.y);
}

void iVec2::operator+= (const iVec2 add) {
    this->x += add.x;
    this->y += add.y;
}

iVec2 iVec2::operator- (const iVec2 sub) const {
    return iVec2(this->x - sub.x, this->y - sub.y);
}

void iVec2::operator-= (const iVec2 sub) {
    this->x -= sub.x;
    this->y -= sub.y;
}

iVec2 iVec2::operator<< (const size_t shift) const {
    return iVec2(this->x << shift, this->y << shift);
}

void iVec2::operator<<= (const size_t shift) {
    this->x <<= shift;
    this->y <<= shift;
}

iVec2 iVec2::operator>> (const size_t shift) const {
    uint32 x;
    uint32 y;
    uint32 mask = (uint32_MAX << (32 - shift));
    
    if (this->x < 0) {
        x = (this->x >> shift) | mask;
    } else {
        x = this->x >> shift;
    }
    if (this->y < 0) {
        y = (this->y >> shift) | mask;
    } else {
        y = this->y >> shift;
    }
    
    return iVec2(x, y);
}

void iVec2::operator>>= (const size_t shift) {
    uint32 mask = (uint32_MAX << (32 - shift));
    if (this->x < 0) {
        this->x = (this->x >> shift) | mask;
    } else {
        this->x = this->x >> shift;
    }
    if (this->y < 0) {
        this->y = (this->y >> shift) | mask;
    } else {
        this->y = this->y >> shift;
    }
}

iVec2::iVec2 (const sf::Vector2i set) {
    this->x = set.x;
    this->y = set.y;
}

void iVec2::operator= (const sf::Vector2i set) {
    this->x = set.x;
    this->y = set.y;
}

sf::Vector2f iVec2::as_vec2f () const {
    return sf::Vector2f(this->x, this->y);
}

sf::Vector2f iVec2::as_vec2f (const float scale) const {
    return sf::Vector2f(this->x, this->y) * scale;
}

double iVec2::distance () const { // distance to (0|0)
    return sqrt(pow((double)this->x, 2) + pow((double)this->y, 2));
}

double iVec2::distance (const iVec2 to) const { // distance to given point
    iVec2 tmp = *this - to;
    return sqrt(pow((double)tmp.x, 2) + pow((double)tmp.y, 2));
}

uint64_t iVec2::fast_distance (const iVec2 to) const {
    uint64_t x = abs(this->x - to.x);
    uint64_t y = abs(this->y - to.y);
    return x < y ? y : x;
}

// END struct iVec2:
