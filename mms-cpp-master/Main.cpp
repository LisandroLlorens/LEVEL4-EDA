#include <iostream>
#include <string>

#include "API.h"

#include <stack>
#include <queue>

#define MAX_SIZE 16
#define GO 'g'
#define RETURN 'r'

typedef struct cells{
    bool mark;
    unsigned int distance;
    int x, y;
    bool wallNorth;
    bool wallEast;
    bool wallWest;
    bool wallSouth;
    bool mouseMark;
}cells_t;

typedef struct mouse{
    int x , y;
    char direction;
} mouse_t;

typedef struct {
    cells_t cells[MAX_SIZE][MAX_SIZE];
    mouse_t mouse;
}board_t;

typedef struct {
    cells_t cell;
    char direction;
}move_t;

char whichDir(char lastTurn, char direction);
void updateCoords(int* x, int* y, char direction);
board_t initBoard(void);
void floodFill(board_t* board, char mode);
void travesti(board_t* board);
char bestMove(board_t* board);

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

int main(int argc, char* argv[]) {
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "abc");

    board_t board = initBoard();
    floodFill(&board, GO);
    travesti(&board);




}



char whichDir(char lastTurn, char direction) {
    switch (direction) {
        case 'n':
            if (lastTurn == 'l' || lastTurn == 'L') {
                direction = 'w';
            }
            else if (lastTurn == 'r' || lastTurn == 'R') {
                direction = 'e';
            }
            break;
        case 'w':
            if (lastTurn == 'l' || lastTurn == 'L') {
                direction = 's';
            }
            else if (lastTurn == 'r' || lastTurn == 'R') {
                direction = 'n';
            }
            break;
        case 's':
            if (lastTurn == 'l' || lastTurn == 'L') {
                direction = 'e';
            }
            else if (lastTurn == 'r' || lastTurn == 'R') {
                direction = 'w';
            }
            break;
        case 'e':
            if (lastTurn == 'l' || lastTurn == 'L') {
                direction = 'n';
            }
            else if (lastTurn == 'r' || lastTurn == 'R') {
                direction = 's';
            }
            break;
        default:
            break;
    }
    return direction;
}

void updateCoords(int* x, int* y, char direction) {
    switch (direction) {
        case 'n':
            (*y)++;
            break;
        case 'w':
            (*x)--;
            break;
        case 'e':
            (*x)++;
            break;
        case 's':
            (*y)--;
            break;
    }
}

board_t initBoard(void) {
    board_t board;
    board.mouse.direction = 'n';
    board.mouse.x = 0;
    board.mouse.y = 0;

    for(int i = 0; i < MAX_SIZE ; i++) {
        for(int j = 0; j < MAX_SIZE ; j++) {//sets boundary walls
            board.cells[i][j].wallNorth = 0;
            board.cells[i][j].wallEast = 0;
            board.cells[i][j].wallSouth = 0;
            board.cells[i][j].wallWest = 0;
            board.cells[i][j].mouseMark = 0;

            if(i == 0) {
                board.cells[i][j].wallWest = 1;
                API::setWall(i, j, 'w');
            }
            else if (i == MAX_SIZE - 1) {
                board.cells[i][j].wallEast = 1;
                API::setWall(i, j, 'e');
            }
            if(j == 0) {
                board.cells[i][j].wallSouth = 1;
                API::setWall(i, j, 's');
            }
            else if (j == MAX_SIZE - 1) {
                board.cells[i][j].wallNorth = 1;
                API::setWall(i, j, 'n');
            }

            board.cells[i][j].x = i;
            board.cells[i][j].y = j;


        }

    }
    return board;
}


void floodFill(board_t* board, char mode){
    for(int i = 0; i < MAX_SIZE ; i++) {
        for(int j = 0; j < MAX_SIZE; j++) {
            board->cells[i][j].mark = false;// resets path marks
        }
    }

    std::queue<cells_t*> pathQ;
    switch (mode) {
        case GO:
            board->cells[7][7] = {1, 0, 7, 7};
            pathQ.push(&(board->cells[7][7]));
            board->cells[8][7] = {1, 0, 8, 7};
            pathQ.push(&(board->cells[8][7]));
            board->cells[7][8] = {1, 0, 7, 8};
            pathQ.push(&(board->cells[7][8]));
            board->cells[8][8] = {1, 0, 8, 8};
            pathQ.push(&(board->cells[8][8]));
            break;

        case RETURN:
            board->cells[0][0] = {1, 0, 0, 0};
            pathQ.push(&(board->cells[0][0]));
            break;
    }

    cells_t* thisCell;

    while(!pathQ.empty()) {

        thisCell = pathQ.front();
        pathQ.pop();

        if(!thisCell->wallNorth && !board->cells[thisCell->x][thisCell->y + 1].mark && thisCell->y + 1 < MAX_SIZE) {//north neighbour
            pathQ.push(&(board->cells[thisCell->x][thisCell->y + 1]));
            board->cells[thisCell->x][thisCell->y + 1].mark = 1;
            board->cells[thisCell->x][thisCell->y + 1].distance = thisCell->distance + 1;

            API::setText(thisCell->x, thisCell->y + 1, std::to_string(thisCell->distance + 1));
        }
        if(!thisCell->wallSouth && !board->cells[thisCell->x][thisCell->y - 1].mark && thisCell->y - 1 >= 0) {//south neighbour
            pathQ.push(&(board->cells[thisCell->x][thisCell->y - 1]));
            board->cells[thisCell->x][thisCell->y - 1].mark = 1;
            board->cells[thisCell->x][thisCell->y - 1].distance = thisCell->distance + 1;

            API::setText(thisCell->x, thisCell->y - 1, std::to_string(thisCell->distance + 1));
        }
        if(!thisCell->wallWest && !board->cells[thisCell->x - 1][thisCell->y].mark && thisCell->x - 1 >= 0) {//west neighbour
            pathQ.push(&(board->cells[thisCell->x - 1][thisCell->y]));
            board->cells[thisCell->x - 1][thisCell->y].mark = 1;
            board->cells[thisCell->x - 1][thisCell->y].distance = thisCell->distance + 1;

            API::setText(thisCell->x - 1, thisCell->y, std::to_string(thisCell->distance + 1));
        }
        if(!thisCell->wallEast && !board->cells[thisCell->x + 1][thisCell->y].mark && thisCell->x + 1 < MAX_SIZE) {//east neighbour
            pathQ.push(&(board->cells[thisCell->x+ 1][thisCell->y]));
            board->cells[thisCell->x + 1][thisCell->y].mark = 1;
            board->cells[thisCell->x + 1][thisCell->y].distance = thisCell->distance + 1;
            API::setText(thisCell->x + 1, thisCell->y, std::to_string(thisCell->distance +1));
        }

    }

}
void travesti(board_t* board) {
    char auxFace;
    bool stuck = false;
    while(!stuck) {
        switch (bestMove(board)) {
            case 'f':
                API::moveForward();
                break;
            case 'r':
                API::turnRight();
                API::moveForward();
                board->mouse.direction = whichDir('r', board->mouse.direction);
                break;
            case 'l':
                API::turnLeft();
                API::moveForward();
            board->mouse.direction = whichDir('l', board->mouse.direction);
                break;
            case 'x':
                stuck = true;
                break;
        }
        if(API::wallRight()) {
            auxFace = whichDir('r', board->mouse.direction);
            API::setWall(board->mouse.x, board->mouse.y, auxFace);
        }
        if (API::wallLeft()){
            auxFace = whichDir('l', board->mouse.direction);
            API::setWall(board->mouse.x, board->mouse.y, auxFace);
        }

        if (API::wallFront()){
            API::setWall(board->mouse.x, board->mouse.y, board->mouse.direction);
        }
        if(!stuck) {
            updateCoords(&board->mouse.x, &board->mouse.y, board->mouse.direction);
            API::setColor(board->mouse.x, board->mouse.y, 'G');
        }

    }



}



char bestMove(board_t* board) {
    int minDist = board->cells[board->mouse.x][board->mouse.y].distance;
    char chosenMove = 'x', tempDir;
    int tempDist;
    int tempX = board->mouse.x, tempY = board->mouse.y;

    if(!API::wallFront()) {
        updateCoords(&tempX, &tempY, board->mouse.direction);
        if(minDist > board->cells[tempX][tempY].distance) {
            minDist = board->cells[tempX][tempY].distance;
            chosenMove = 'f';
        }
    }
    if(!API::wallLeft()) {
        tempX = board->mouse.x;
        tempY = board->mouse.y;
        tempDir = whichDir('l', board->mouse.direction);

        updateCoords(&tempX, &tempY, tempDir);
        if(minDist > board->cells[tempX][tempY].distance) {
            minDist = board->cells[tempX][tempY].distance;
            chosenMove = 'l';
        }

    }
    if(!API::wallRight()) {
        tempX = board->mouse.x;
        tempY = board->mouse.y;
        tempDir = whichDir('r', board->mouse.direction);

        updateCoords(&tempX, &tempY, tempDir);
        if(minDist > board->cells[tempX][tempY].distance) {
            minDist = board->cells[tempX][tempY].distance;
            chosenMove = 'r';
        }
    }
    return chosenMove;
}


