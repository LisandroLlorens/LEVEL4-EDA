#include <iostream>
#include <string>

#include "API.h"

enum{N = 1, W, S, E} ;

int whichDir(char lastTurn, int direction);
void updateCoords(int* x, int* y, int direction);

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

int main(int argc, char* argv[]) {
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "abc");
    int x = 0 , y = 0;
    int facing = N;
    while (true) {
        if (!API::wallLeft()) {
            API::turnLeft();
            facing = whichDir('l', facing);

        }
        while (API::wallFront()) {
            API::turnRight();
            facing = whichDir('r', facing);
        }

        updateCoords(&x, &y, facing);
        API::setColor(x, y, 'b');
        API::moveForward();
    }
}






int whichDir(char lastTurn, int direction) {
    switch (direction) {
        case N:
            if (lastTurn == 'l' || lastTurn == 'L') {
                direction = W;
            }
            else if (lastTurn == 'r' || lastTurn == 'R') {
                direction = E;
            }
            break;
        case W:
            if (lastTurn == 'l' || lastTurn == 'L') {
                direction = S;
            }
            else if (lastTurn == 'r' || lastTurn == 'R') {
                direction = N;
            }
            break;
        case S:
            if (lastTurn == 'l' || lastTurn == 'L') {
                direction = E;
            }
            else if (lastTurn == 'r' || lastTurn == 'R') {
                direction = W;
            }
            break;
        case E:
            if (lastTurn == 'l' || lastTurn == 'L') {
                direction = N;
            }
            else if (lastTurn == 'r' || lastTurn == 'R') {
                direction = S;
            }
            break;
        default:
            break;
    }
    return direction;
}

void updateCoords(int* x, int* y, int direction) {
    switch (direction) {
        case N:
            (*y)++;
            break;
        case W:
            (*x)--;
            break;
        case E:
            (*x)++;
            break;
        case S:
            (*y)--;
            break;
    }
}
