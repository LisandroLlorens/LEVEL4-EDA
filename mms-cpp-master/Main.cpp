#include <complex>
#include <iostream>
#include <string>
#include "API.h"
#include "mightyMouseAI.h"

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

int main(int argc, char* argv[]) {
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "abc");

    board_t board = initBoard();
    int tripsLeft = 5;
    char mode = GO;

    while (tripsLeft > 0) { //Go and return 5 times; halting after the 5th (3rd time going towards center)
        while(board.cells[board.mouse.x][board.mouse.y].distance > 0) { //While it has not arrived to its current destination
            floodFill(&board, mode);
            traverse(&board);
        }
        mode = (mode == GO)? RETURN : GO; //Switches from trying to reach the center (GO), to trying to reach the start (RETURN)
        floodFill(&board, mode);
        tripsLeft--;
    }
}




