#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <sstream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>



using namespace std;



#define MAP_FILE "map0.tsv"
#define POS_FILE "pos0_0.tsv"
#define RAND_POS 1



class Map{
public:
    vector<vector<char>> map;
    tuple<int, int> E;  // Exit
    tuple<int, int> H;  // Hero
    tuple<int, int> D;  // Dragon
    tuple<int, int> K;  // Key
    bool Key;
    bool Win;


// RandGen
    tuple<int, int> generatePos(){
        while(1){
            int xGen = rand() % map.size();
            int yGen = rand() % map[0].size();
            if(map[xGen][yGen] == ' '){
                return {xGen, yGen};
            }
        }
    }


    tuple<int, int> generatePosDragon(){
        while(1){
            int xGenD = rand() % map.size();
            int yGenD = rand() % map[0].size();
            if(map[xGenD][yGenD] == ' '
                && abs(xGenD - get<0>(E)) + abs(yGenD - get<1>(E)) > 2
                && abs(xGenD - get<0>(H)) + abs(yGenD - get<1>(H)) > 2
                && abs(xGenD - get<0>(K)) + abs(yGenD - get<1>(K)) > 2){
                return {xGenD, yGenD};
            }
        }
    }


// Constructor
    Map(){
//        printw("Loading Map\n\n");
//        refresh();

        // Load map from file
        ifstream mapfile(MAP_FILE);
        string line;
        while(getline(mapfile, line)){
            vector<char> mapline;
            for(int i = 0; i < line.size(); i++){
                if(line[i] == 'X') mapline.push_back(line[i]);
                else if(line[i] == '.') mapline.push_back(' ');
            }
            map.push_back(mapline);
        }
        mapfile.close();

        // Load positions from file
        ifstream posfile(POS_FILE);
        while(getline(posfile, line)){
            int x, y;
            char c;
            stringstream ss(line);
            ss >> c >> x >> y;
            if(RAND_POS){
                if(c == 'E'){
                    E = {x, y};
                    map[x][y] = c;
                    H = generatePos();
                    map[get<0>(H)][get<1>(H)] = 'H';
                    K = generatePos();
                    map[get<0>(K)][get<1>(K)] = 'K';
                    D = generatePosDragon();
                    map[get<0>(D)][get<1>(D)] = 'D';
                    break;
                }
                else continue;
            }
            else{
                switch(c){
                    case 'E':
                        E = {x, y};
                        break;
                    case 'H':
                        H = {x, y};
                        break;
                    case 'D':
                        D = {x, y};
                        break;
                    case 'K':
                        K = {x, y};
                        break;
                }
            }
            map[x][y] = c;
        }
        posfile.close();

        Key = 0;
        Win = 0;
    }


// Printer
    /**
    * @brief Prints the current state of the game map.
    */
    void printMap() {
        clear();    // Clear the screen before printing the map

        for(vector<char>& linha : map) {
            for(char c : linha) {
                switch(c){
                    case 'X':
                        attron(COLOR_PAIR(2));
                        break;

                    case 'E':
                        attron(COLOR_PAIR(3) | A_BOLD);
                        break;

                    case 'H':
                        attron(COLOR_PAIR(4) | A_BOLD);
                        break;

                    case 'D':
                        attron(COLOR_PAIR(5) | A_BOLD);
                        break;

                    case 'K':
                        attron(COLOR_PAIR(6) | A_BOLD);
                        break;
                }
                addch(' ');
                addch(c);
                addch(' ');

                attrset(A_NORMAL);
            }
            addch('\n');
        }
        refresh();
    }


// Movement functions
    void moveUp(){
    //    printw("Moving Up\n");
    //    refresh();

        int x = get<0>(this->H);
        int y = get<1>(this->H);
        char c = this->map[x - 1][y];

        switch(c){
            case 'X':
                break;

            default:
                if(c == 'E' && !this->Key) break;
                this->map[x][y] = ' ';
                this->H = {x - 1, y};
                this->map[x - 1][y] = 'H';
                if(c == 'K') this->Key = 1;
                else if(c == 'E' && this->Key) this->Win = 1;
        }
    }


    void moveLeft(){
    //    printw("Moving Left\n");
    //    refresh();

        int x = get<0>(this->H);
        int y = get<1>(this->H);

        char c = this->map[x][y - 1];

        switch(c){
            case 'X':
                break;

            default:
                if(c == 'E' && !this->Key) break;
                this->map[x][y] = ' ';
                this->H = {x, y - 1};
                this->map[x][y - 1] = 'H';
                if(c == 'K') this->Key = 1;
                else if(c == 'E' && this->Key) this->Win = 1;
        }
    }


    void moveDown(){
    //    printw("Moving Down\n");
    //    refresh();

        int x = get<0>(this->H);
        int y = get<1>(this->H);

        char c = this->map[x + 1][y];

        switch(c){
            case 'X':
                break;

            default:
                if(c == 'E' && !this->Key) break;
                this->map[x][y] = ' ';
                this->H = {x + 1, y};
                this->map[x + 1][y] = 'H';
                if(c == 'K') this->Key = 1;
                else if(c == 'E' && this->Key) this->Win = 1;
        }
    }


    void moveRight(){
    //    printw("Moving Right\n");
    //    refresh();

        int x = get<0>(this->H);
        int y = get<1>(this->H);

        char c = this->map[x][y + 1];

        switch(c){
            case 'X':
                break;

            default:
                if(c == 'E' && !this->Key) break;
                this->map[x][y] = ' ';
                this->H = {x, y + 1};
                this->map[x][y + 1] = 'H';
                if(c == 'K') this->Key = 1;
                else if(c == 'E' && this->Key) this->Win = 1;
        }
    }


    bool DragonClose(){
        int xH = get<0>(this->H);
        int yH = get<1>(this->H);
        int xD = get<0>(this->D);
        int yD = get<1>(this->D);

        if((abs(xH - xD) == 1 && abs(yH - yD) == 0) || (abs(xH - xD) == 0 && abs(yH - yD) == 1)) return 1;
        return 0;
    }
};



void gameLoop(Map& map){
    int input;
    while(1){
        map.printMap();

        if(map.Key){
            attron(COLOR_PAIR(1));
            printw("\nYou picked up the key!\n");
            attroff(COLOR_PAIR(1));
            refresh();
        }

        input = getch();

        switch(input){
            case 'W':
            case 'w':
            case KEY_UP:
                map.moveUp();
                break;

            case 'A':
            case 'a':
            case KEY_LEFT:
                map.moveLeft();
                break;

            case 'S':
            case 's':
            case KEY_DOWN:
                map.moveDown();
                break;

            case 'D':
            case 'd':
            case KEY_RIGHT:
                map.moveRight();
                break;
        }

        if(map.DragonClose()){
            attron(COLOR_PAIR(1));
            printw("\nSee that big guy over there?\n\tYeah, you dead...\n");
            attroff(COLOR_PAIR(1));
            refresh();
            break;
        }

        if(input == 'Q' || input == 'q'){
            attron(COLOR_PAIR(1));
            printw("\nQuitting...\n");
            attroff(COLOR_PAIR(1));
            refresh();
            break;
        }

        if(map.Win) {
            clear();
            map.printMap();
            attron(COLOR_PAIR(1) | A_BOLD);
            printw("\nYou Win!\n");
            attroff(COLOR_PAIR(1) | A_BOLD);
            refresh();
            break;
        }
    }
}



int main(){
    srand(time(NULL));

    initscr();              // Initialize ncurses
    cbreak();               // Allow command signals to be sent to the program
    keypad(stdscr, TRUE);   // Enable special keys to be captured (interest in arrows)
    noecho();               // Don't echo input
    start_color();          // Enable color functionality

    init_color(COLOR_BLACK, 0, 0, 0);   // Set black color to RGB(0, 0, 0)

    init_pair(1, -1, -1);   // Initialize color pair for text
    init_pair(2, COLOR_WHITE, COLOR_WHITE);     // Initialize color pair for walls X
    init_pair(3, COLOR_GREEN, COLOR_GREEN);     // Initialize color pair for exit E
    init_pair(4, COLOR_CYAN, COLOR_CYAN);       // Initialize color pair for hero H
    init_pair(5, COLOR_RED, COLOR_RED);         // Initialize color pair for dragon D
    init_pair(6, COLOR_YELLOW, COLOR_YELLOW);   // Initialize color pair for key K

    attron(COLOR_PAIR(1));
    printw("Dragon's Bane\n\n~~~Press any key to start~~~");
    attroff(COLOR_PAIR(1));
    refresh();
    getch();    // Wait for user input before starting the game

    Map map;

    gameLoop(map);

    attron(COLOR_PAIR(1));
    printw("\n~~~Press any key to exit~~~\n");
    attroff(COLOR_PAIR(1));
    refresh();
    getch();    // Wait for user input before exiting
    endwin();   // End ncurses mode
    return 0;
}
