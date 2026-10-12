#include "Game.hpp"

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

namespace logic{


Game::Game(std::string map_file, std::string pos_file, bool rand_pos, int dragonCount):
    map(map_file),
    hero(-1, -1),
    sword(-1, -1),
    dragonCount(dragonCount),
    kills(0),
    key(-1, -1),
    exit(-1, -1),
    Win(false),
    GameOver(false)
{
    std::ifstream posFile(pos_file);
    std::string line;
    while(std::getline(posFile, line)){
        int x, y;
        char c;
        std::stringstream ss(line);
        ss >> c >> x >> y;
        if(rand_pos){
            if(c == 'E'){
                exit.setPosition(x, y);
                std::vector<int> pos = generatePos();
                hero.setPosition(pos[0], pos[1]);
                pos = generatePos();
                sword.setPosition(pos[0], pos[1]);
                //key.setPosition(pos[0], pos[1]);
                createDragons();
                break;
            }
            continue;
        }
        else{
            switch(c){
                case 'H':
                hero.setPosition(x, y);
                break;

                case 'D':
                //dragon.setPosition(x, y);
                break;

                case 'S':
                sword.setPosition(x, y);
                break;

                case 'K':
                key.setPosition(x, y);
                break;

                case 'E':
                exit.setPosition(x, y);
                break;
            }
        }
    }
}


void Game::createDragons(){
    for(int i = 0; i < dragonCount; i++){
        std::vector<int> pos = generatePosDragon();
        dragons.push_back(Dragon(pos[0], pos[1]));
    }
}


std::vector<int> Game::generatePos(){
    while(true){
        int xGen = rand() % map.getSize()[0];
        int yGen = rand() % map.getSize()[1];
        if(getCell(xGen, yGen) == ' '){
            return {xGen, yGen};
        }
    }
}


std::vector<int> Game::generatePosDragon(){
    while(true){
        int xGenD = rand() % map.getSize()[0];
        int yGenD = rand() % map.getSize()[1];
        if(getCell(xGenD, yGenD) == ' '
            && abs(xGenD - hero.getPosition()[0]) + abs(yGenD - hero.getPosition()[1]) >= 2
            && abs(xGenD - sword.getPosition()[0]) + abs(yGenD - sword.getPosition()[1]) >= 2
            && abs(xGenD - key.getPosition()[0]) + abs(yGenD - key.getPosition()[1]) >= 2
            && abs(xGenD - exit.getPosition()[0]) + abs(yGenD - exit.getPosition()[1]) >= 2)
        {
            return {xGenD, yGenD};
        }
    }
}


std::vector<int> Game::getMapSize(){return map.getSize();}


char Game::getCell(int x, int y){
    if(hero.getPosition() == std::vector<int>{x, y}) return hero.getSymbol();
    for(Dragon& dragon : dragons){
        if(dragon.getPosition() == std::vector<int>{x, y}){
            if(sword.getPosition() == std::vector<int>{x, y}) return 'F';
            return dragon.getSymbol();
        }
    }
    if(sword.getPosition() == std::vector<int>{x, y}) return 'S';
    if(key.getPosition() == std::vector<int>{x, y}) return 'K';
    if(exit.getPosition() == std::vector<int>{x, y}) return 'E';
    if(map.isWall(x, y)) return 'X';   // At last so that the walls don't overwrite the other elements
    return ' ';
}


int Game::isDragonClose(){
    int xH = hero.getPosition()[0];
    int yH = hero.getPosition()[1];

    for(int d = 0; d < dragons.size(); d++){
        int xD = dragons[d].getPosition()[0];
        int yD = dragons[d].getPosition()[1];
        if((abs(xH - xD) + abs(yH - yD)) <= 1) return d;
    }

    return -1;
}


bool Game::isHeroArmed(){return hero.IsArmed();}

bool Game::isSwordPickedUp(){return sword.IsPickedUp();}

int Game::getDragonCount(){return dragonCount;}

int Game::getKills(){return kills;}

bool Game::isKeyPickedUp(){return key.IsPickedUp();}

bool Game::isExitOpened(){return exit.IsOpened();}

bool Game::isWin(){return Win;}

bool Game::isGameOver(){return GameOver;}


void Game::killDragon(int index){
    dragons[index].Kill();
    kills++;
}


void Game::moveHero(char direction){
    int x = hero.getPosition()[0];
    int y = hero.getPosition()[1];
    char c;
    switch(direction){
        case 'U':
            c = getCell(x - 1, y);
            if(c == 'X' || (c == 'E' && !exit.IsOpened())) return;
            hero.setPosition(x - 1, y);
            break;

        case 'D':
            c = getCell(x + 1, y);
            if(c == 'X' || (c == 'E' && !exit.IsOpened())) return;
            hero.setPosition(x + 1, y);
            break;

        case 'L':
            c = getCell(x, y - 1);
            if(c == 'X' || (c == 'E' && !exit.IsOpened())) return;
            hero.setPosition(x, y - 1);
            break;

        case 'R':
            c = getCell(x, y + 1);
            if(c == 'X' || (c == 'E' && !exit.IsOpened())) return;
            hero.setPosition(x, y + 1);
            break;
    }

    if(c == 'S'){
        sword.PickUp();
        hero.Arm();
    }
    else if(c == 'K'){
        key.PickUp();
        exit.Open();
    }
    else if(c == 'E' && exit.IsOpened()) Win = true;
}


void Game::moveDragons(){
    for(Dragon& dragon : dragons){
        if(!dragon.IsAlive()) continue;

        if(!dragon.IsAwake()){
            dragon.SleepTurn();
            continue;
        }

        // Randomy put the dragon to sleep with a 1/5 chance
        if(rand() % 5 == 0){
            dragon.GoToSleep(rand() % 3 + 1);   // Sleep for 1-3 turns
            continue;
        }

        int x = dragon.getPosition()[0];
        int y = dragon.getPosition()[1];

        // Check valid moves
        std::vector<std::vector<int>> validMoves;
        if(getCell(x - 1, y) != 'X' && getCell(x - 1, y) != 'E' && getCell(x - 1, y) != 'D' && getCell(x - 1, y) != 'd') validMoves.push_back({x - 1, y});
        if(getCell(x + 1, y) != 'X' && getCell(x + 1, y) != 'E' && getCell(x + 1, y) != 'D' && getCell(x - 1, y) != 'd') validMoves.push_back({x + 1, y});
        if(getCell(x, y - 1) != 'X' && getCell(x, y - 1) != 'E' && getCell(x, y - 1) != 'D' && getCell(x - 1, y) != 'd') validMoves.push_back({x, y - 1});
        if(getCell(x, y + 1) != 'X' && getCell(x, y + 1) != 'E' && getCell(x, y + 1) != 'D' && getCell(x - 1, y) != 'd') validMoves.push_back({x, y + 1});

        // Choose a random valid move
        if(!validMoves.empty()){
            int randMove = rand() % validMoves.size();
            dragon.setPosition(validMoves[randMove][0], validMoves[randMove][1]);
        }
    }
}


void Game::encounter(){
    int d = isDragonClose();
    if(d != -1){
        if(hero.IsArmed()) killDragon(d);
        else if(dragons[d].IsAwake()) GameOver = true;
    }

    if(kills >= dragonCount) exit.Open();
}

}
