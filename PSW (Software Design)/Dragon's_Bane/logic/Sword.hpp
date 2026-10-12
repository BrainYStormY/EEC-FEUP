#pragma once

#include "Element.hpp"

namespace logic{

class Sword : public Element{
private:
    bool isPickedUp;

public:
// Constructor
    Sword(int x, int y):
        Element(x, y, 'S'),
        isPickedUp(false)
    {}
    
// Getter
    bool IsPickedUp(){
        return isPickedUp;
    }

// Setter
    void PickUp(){
        setPosition(-1, -1);
        isPickedUp = true;
    }
};

}