#pragma once

#include "Element.hpp"

namespace logic{

class Hero : public Element{
private:
    bool isArmed;

public:
// Constructor
    Hero(int x, int y):
        Element(x, y, 'H'),
        isArmed(false)
    {}

// Getter
    bool IsArmed(){
        return isArmed;
    }

// Setter
    void Arm(){
        isArmed = true;
        symbol = 'A';
    }
};

}
