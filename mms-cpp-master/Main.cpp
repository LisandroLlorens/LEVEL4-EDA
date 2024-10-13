#include <complex>
#include <iostream>
#include <string>
#include <queue>

#include "API.h"

#define MAX_SIZE 16 //Height, width of the maze
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
void traverse(board_t* board);
char bestMove(board_t* board);
void setWallDir(char dir, board_t* board);

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

    while (tripsLeft) { //Go and return 5 times; halting after the 5th (3rd time going towards center)
        while(board.cells[board.mouse.x][board.mouse.y].distance) { //While it has not arrived to its current destination
            floodFill(&board, mode);
            traverse(&board);
        }
        mode = (mode == GO)? RETURN : GO; //Switches from trying to reach the center (GO), to trying to reach the start (RETURN)
        tripsLeft--;
    }
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
        default:
            break;
    }
}

board_t initBoard(void) {
    board_t board;

    //initializes mouse
    board.mouse.direction = 'n';
    board.mouse.x = 0;
    board.mouse.y = 0;

    for(int i = 0; i < MAX_SIZE ; i++) {
        for(int j = 0; j < MAX_SIZE ; j++) {
            board.cells[i][j].wallNorth = 0;
            board.cells[i][j].wallEast = 0;
            board.cells[i][j].wallSouth = 0;
            board.cells[i][j].wallWest = 0;

            //Setting boundary walls
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

            //Setting x,y coordinates
            board.cells[i][j].x = i;
            board.cells[i][j].y = j;
        }

    }
    return board;
}


void floodFill(board_t* board, char mode) {
    log("Starting floodFill...\n");

    for(int i = 0; i < MAX_SIZE ; i++) {
        for(int j = 0; j < MAX_SIZE; j++) {
            board->cells[i][j].mark = false;// resets path marks
        }
    }
    std::queue<cells_t*> pathQ;

    //Switches between setting the center or the start as the objective
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

        default:
            break;
    }

    cells_t* thisCell;

    while(!pathQ.empty()) {

        // Takes the first cell in the queue and enqueues all of its neighbours for future analysis of their neighbours;
        // sets their marks to 1 to signify that they have been analyzed and sets their distance to be equal to the original cell plus one.

        // Neighbours are considered valid if they are in the maze, are not marked and there is no wall separating them from the original cell.

        thisCell = pathQ.front();

        if(!(thisCell->wallNorth) && !board->cells[thisCell->x][thisCell->y + 1].mark && thisCell->y + 1 < MAX_SIZE) {//north neighbour
            pathQ.push(&(board->cells[thisCell->x][thisCell->y + 1]));
            board->cells[thisCell->x][thisCell->y + 1].mark = 1;
            board->cells[thisCell->x][thisCell->y + 1].distance = thisCell->distance + 1;

            API::setText(thisCell->x, thisCell->y + 1, std::to_string(thisCell->distance + 1));
        }
        if(!(thisCell->wallSouth) && !board->cells[thisCell->x][thisCell->y - 1].mark && thisCell->y - 1 >= 0) {//south neighbour
            pathQ.push(&(board->cells[thisCell->x][thisCell->y - 1]));
            board->cells[thisCell->x][thisCell->y - 1].mark = 1;
            board->cells[thisCell->x][thisCell->y - 1].distance = thisCell->distance + 1;

            API::setText(thisCell->x, thisCell->y - 1, std::to_string(thisCell->distance + 1));
        }
        if(!(thisCell->wallWest) && !board->cells[thisCell->x - 1][thisCell->y].mark && thisCell->x - 1 >= 0) {//west neighbour
            pathQ.push(&(board->cells[thisCell->x - 1][thisCell->y]));
            board->cells[thisCell->x - 1][thisCell->y].mark = 1;
            board->cells[thisCell->x - 1][thisCell->y].distance = thisCell->distance + 1;

            API::setText(thisCell->x - 1, thisCell->y, std::to_string(thisCell->distance + 1));
        }
        if(!(thisCell->wallEast) && !board->cells[thisCell->x + 1][thisCell->y].mark && thisCell->x + 1 < MAX_SIZE) {//east neighbour
            pathQ.push(&(board->cells[thisCell->x+ 1][thisCell->y]));
            board->cells[thisCell->x + 1][thisCell->y].mark = 1;
            board->cells[thisCell->x + 1][thisCell->y].distance = thisCell->distance + 1;
            API::setText(thisCell->x + 1, thisCell->y, std::to_string(thisCell->distance +1));
        }
        // dequeues the cell
        pathQ.pop();

    }

}
void traverse(board_t* board) {
    log("Traversing Maze...\n");
    //Used to properly set walls without rotating the mouse
    char auxFace;

    //Boolean flags: representing whether the mouse is stuck and whether it is its first step since it last got stuck
    bool stuck = false;
    bool first = true;

    while(!stuck) {

        //Record walls that the mouse runs into
        if(API::wallRight()) {
            auxFace = whichDir('r', board->mouse.direction);
            setWallDir(auxFace, board);
        }
        if (API::wallLeft()){
            auxFace = whichDir('l', board->mouse.direction);
            setWallDir(auxFace, board);
        }

        if (API::wallFront()){
            setWallDir(board->mouse.direction, board);
        }

        //Choose the best move based on reducing the flood fill distance value
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
            case 'x': //If the mouse finds itself stuck it might be...
                //In a corner where it needs to turn around
                if(first) {
                    API::turnLeft();
                    API::turnLeft();
                    board->mouse.direction = whichDir('l', board->mouse.direction);
                    board->mouse.direction = whichDir('l', board->mouse.direction);
                    API::moveForward();
                }
                //Or genuinely stuck
                else {
                    stuck = true;
                }
                break;
            default:
                break;
        }
        if(!stuck) { //Updates the tiles visually during the time the mouse traverses and is not stuck
            updateCoords(&board->mouse.x, &board->mouse.y, board->mouse.direction);
            API::setColor(board->mouse.x, board->mouse.y, 'G');
        }
        first = false;
    }
}



char bestMove(board_t* board) {
    //Minimum distance from objective is initialized as current distance. Chosen move is initialized as stuck
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


void setWallDir(char dir, board_t* board) {
    //Sets walls both visually and in the board matrix based on absolute directions; hence why it needs to
    //receive the mouse´s direction

    //if statement ensures that walls are set in both adjacent cells
    switch (dir) {
        case 'n':
            board->cells[board->mouse.x][board->mouse.y].wallNorth = true;
            if(board->mouse.y + 1 < MAX_SIZE) {
                board->cells[board->mouse.x][board->mouse.y + 1].wallSouth = true;
            }
            break;
        case 's':
            board->cells[board->mouse.x][board->mouse.y].wallSouth = true;
            if(board->mouse.y - 1 >= 0) {
                board->cells[board->mouse.x][board->mouse.y - 1].wallNorth = true;
            }
            break;
        case 'e':
            board->cells[board->mouse.x][board->mouse.y].wallEast = true;
            if(board->mouse.x + 1 < MAX_SIZE) {
                board->cells[board->mouse.x + 1][board->mouse.y].wallWest = true;
            }
            break;
        case 'w':
            board->cells[board->mouse.x][board->mouse.y].wallWest = true;
            if(board->mouse.x - 1 >= 0) {
                board->cells[board->mouse.x - 1][board->mouse.y].wallEast = true;
            }
            break;
        default:
            break;
    }
    API::setWall(board->mouse.x, board->mouse.y, dir);
}

