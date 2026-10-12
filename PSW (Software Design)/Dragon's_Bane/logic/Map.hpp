#pragma once

#include <vector>
#include <string>
#include <fstream>

namespace logic{

class Map{
private:
    std::vector<std::vector<char>> map;

public:
// Constructor
    Map(std::string map_file){
        std::ifstream mapFile(map_file);
        std::string line;
        while(std::getline(mapFile, line)){
            std::vector<char> mapLine;
            for(int i = 0; i < line.size(); i++){
                if(line[i] == 'X') mapLine.push_back(line[i]);
                else if(line[i] == '.') mapLine.push_back(' ');
            }
            map.push_back(mapLine);
        }
        mapFile.close();
    }

// Getters
    bool isWall(int x, int y){
        return map[x][y] == 'X';
    }

    std::vector<int> getSize(){
        return {(int)map.size(), (int)map[0].size()};
    }
};

}
