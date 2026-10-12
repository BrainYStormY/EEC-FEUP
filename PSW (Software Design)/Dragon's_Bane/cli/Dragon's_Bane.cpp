#include <cstdlib>
#include <ctime>
#include <ncurses.h>

#include "../logic/Game.hpp"

using namespace logic;

#define MAP_FILE "map0.tsv"
#define POS_FILE "pos0_0.tsv"
#define RAND_POS true



void printGame(Game& game){
    for(int i = 0; i < game.getMapSize()[0]; i++){
        for(int j = 0; j < game.getMapSize()[1]; j++){
            if(game.getCell(i, j) == 'X') attron(COLOR_PAIR(1));
            else if(game.getCell(i, j) == 'H') attron(COLOR_PAIR(2) | A_BOLD);
            else if(game.getCell(i, j) == 'A' || game.getCell(i, j) == 'S') attron(COLOR_PAIR(3) | A_BOLD);
            else if(game.getCell(i, j) == 'D' || game.getCell(i, j) == 'd' || game.getCell(i, j) == 'F') attron(COLOR_PAIR(4) | A_BOLD);
            else if(game.getCell(i, j) == 'K') attron(COLOR_PAIR(5) | A_BOLD);
            else if(game.getCell(i, j) == 'E') attron(COLOR_PAIR(6) | A_BOLD);
            printw(" %c ", game.getCell(i, j));
            attrset(A_NORMAL);
        }
        printw("\n");
    }
    refresh();
}



int GameEnd(Game& game){
    if(game.isGameOver()){
        clear();
        printGame(game);
        printw("\nSee that big guy over there?\n\tYeah, you dead...\n");
        refresh();
        return 1;
    }

    if(game.isWin()){
        clear();
        printGame(game);
        printw("\nYou Win!\n");
        refresh();
        return 1;
    }

    return 0;
}



int main(){
    srand(time(NULL));

    initscr();              // Initialize ncurses
    cbreak();               // Allow command signals to be sent to the program
    keypad(stdscr, TRUE);   // Enable special keys to be captured (interest in arrows and Esc)
    ESCDELAY = 0;           // Set the delay for the Esc key to 0 milliseconds
    noecho();               // Don't echo input
    start_color();          // Enable color functionality

    init_color(COLOR_BLACK, 0, 0, 0);   // Set black color to RGB(0, 0, 0)

    init_pair(1, COLOR_WHITE, COLOR_WHITE);     // Initialize color pair for walls X
    init_pair(2, COLOR_CYAN, COLOR_CYAN);       // Initialize color pair for (unarmed) hero H
    init_pair(3, COLOR_BLUE, COLOR_BLUE);       // Initialize color pair for armed hero A or sword S
    init_pair(4, COLOR_RED, COLOR_RED);         // Initialize color pair for dragon D
    init_pair(5, COLOR_YELLOW, COLOR_YELLOW);   // Initialize color pair for key K
    init_pair(6, COLOR_GREEN, COLOR_GREEN);     // Initialize color pair for exit E

    printw("Dragon's Bane\n\n~~~Press any key to start~~~");
    refresh();
    getch();    // Wait for user input before starting the game

    clear();
    printw("How many dragons do you want to fight?\n");
    refresh();
    echo();     // Enable echoing of input for user to see what they type
    int dragCount;
    scanw("%d", &dragCount);
    noecho();   // Disable echoing of input after reading the number of dragons

    Game game(MAP_FILE, POS_FILE, RAND_POS, dragCount);

    int input;

    while(true){
        clear();

        printGame(game);

                if(game.isKeyPickedUp()){
            printw("\nYou picked up the key!\nHurry up to the exit!\n");
            refresh();
        }

        if(game.isSwordPickedUp()){
            if(game.getKills() != game.getDragonCount()) printw("\nYou picked up the sword!\nNow slay the dragons!\t%d/%d dragons killed\n", game.getKills(), game.getDragonCount());
            else printw("\nAll dragons killed!\nHurry up to the exit!\n");
            refresh();
        }

        input = getch();

        switch(input){
            case 'W':
            case 'w':
            case KEY_UP:
                game.moveHero('U');
                break;

            case 'S':
            case 's':
            case KEY_DOWN:
                game.moveHero('D');
                break;

            case 'A':
            case 'a':
            case KEY_LEFT:
                game.moveHero('L');
                break;

            case 'D':
            case 'd':
            case KEY_RIGHT:
                game.moveHero('R');
                break;
        }

        if(input == 'Q' || input == 'q' || input == 27){    // 27 is the ASCII code for the Esc key
            printw("\nQuitting...\n");
            refresh();
            break;
        }

        game.encounter();
        if(GameEnd(game)) break;

        game.moveDragons();

        game.encounter();
        if(GameEnd(game)) break;
    }

    printw("\n~~~Press any key to exit~~~\n");
    refresh();
    getch();    // Wait for user input before exiting
    endwin();   // End ncurses mode
    return 0;
}
