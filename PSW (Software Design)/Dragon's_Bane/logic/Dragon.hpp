#pragma once

#include "Element.hpp"

namespace logic{

class Dragon : public Element{
private:
    bool isAlive;
    bool isAwake;
    int sleepTurns;

public:
// Constructor
    Dragon(int x, int y):
        Element(x, y, 'D'),
        isAlive(true),
        isAwake(true),
        sleepTurns(0)
    {}

// Getter
    bool IsAlive(){
        return isAlive;
    }

    bool IsAwake(){
        return isAwake;
    }

// Setter
    void GoToSleep(int turns){
        isAwake = false;
        sleepTurns = turns;
        symbol = 'd';
    }

    void SleepTurn(){
        if(--sleepTurns == 0){
            WakeUp();
        }
    }

    void WakeUp(){
        isAwake = true;
        symbol = 'D';
    }

    void Kill(){
        setPosition(-1, -1);
        isAlive = false;
    }
};

}
