#pragma once

#include "Map.hpp"
#include "Hero.hpp"
#include "Sword.hpp"
#include "Dragon.hpp"
#include "Key.hpp"
#include "Exit.hpp"

namespace logic{

class Game{
private:
    Map map;
    Hero hero;
    Sword sword;
    std::vector<Dragon> dragons;
    int dragonCount;
    int kills;
    Key key;
    Exit exit;
    bool Win;
    bool GameOver;


public:
// Constructor
    Game(std::string map_file, std::string pos_file, bool rand_pos, int dragonCount);

private:
// "Constructor" for dragons
    void createDragons();
// Random Generators
    std::vector<int> generatePos();
    std::vector<int> generatePosDragon();

public:
// Getters
    std::vector<int> getMapSize();
    char getCell(int x, int y);
    int isDragonClose();
    bool isHeroArmed();
    bool isSwordPickedUp();
    int getDragonCount();
    int getKills();
    bool isKeyPickedUp();
    bool isExitOpened();
    bool isWin();
    bool isGameOver();

private:
//Setter
    void killDragon(int index);

public:
// Movement
    void moveHero(char direction);
    void moveDragons();
// Encounter
    void encounter();
};

}
