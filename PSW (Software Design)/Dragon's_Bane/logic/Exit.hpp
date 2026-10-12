#pragma once

#include "Element.hpp"

namespace logic{

class Exit : public Element{
private:
    bool isOpened;

public:
// Constructor
    Exit(int x, int y):
        Element(x, y, 'E'),
        isOpened(false)
    {}

// Getter
    bool IsOpened(){
        return isOpened;
    }

// Setter
    void Open(){
        isOpened = true;
    }
};

}
