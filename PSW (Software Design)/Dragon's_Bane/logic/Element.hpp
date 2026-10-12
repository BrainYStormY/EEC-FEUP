#pragma once

#include <vector>

namespace logic{

class Element{
protected:
    int x;
    int y;
    char symbol;

public:
// Constructor
    Element(int x, int y, char symbol):
        x(x),
        y(y),
        symbol(symbol)
    {}

// Destructor for derived classes
    virtual ~Element() = default;

// Getters
    std::vector<int> getPosition(){
        return {x, y};
    }

    char getSymbol(){
        return symbol;
    }

// Setters
    void setPosition(int newX, int newY){
        x = newX;
        y = newY;
    }

    void setSymbol(char newSymbol){
        symbol = newSymbol;
    }
};

}
