#ifndef MIGHTYMOUSEAI_H
#define MIGHTYMOUSEAI_H

/******************************
 * DEFINE CONSTANTS
 *****************************/
#define MAX_SIZE 16 // Height/width of the maze
#define GO 'g'
#define RETURN 'r'

/******************************
 * TYPEDEFS AND STRUCTS
 *****************************/

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

/******************************
 * FUNCTIONS
 *****************************/

/*
 *  @brief Initializes the board with both its cells
 *         and the mouse in the bottom left corner.
 *         Takes care of activating the border walls.
 *
 *  @return The board in question
 */
board_t initBoard(void);

/*
 *  @brief Utilizes a flood fill algorithm to assign the distance to a set
 *         objective for each cell using the walls that are known/marked already.
 *
 *  @param board The board from where to extract the mouse's current position and
 *         the data regarding each cell and its walls & neighbouring cells.
 *
 *  @param mode Changes the objective. GO is for reaching the middle of the
 *              maze while RETURN is for returning back to the bottom left corner.
 */
void floodFill(board_t* board, char mode);

/*
 *  @brief Allows the mouse to traverse the maze, always lowering its distance to
 *         its current objective. Stops when it gets 'stuck' between walls and/or
 *         higher distance values (which is solved by calling flood fill once more)
 *
 *  @param board The board from where to extract the mouse's current position and
 *         the data regarding each cell and its walls & neighbouring cells.
 */
void traverse(board_t* board);

/*
 *  @brief Logs messages. Used to send messages in the emulator
 *
 *  @param text The message to be sent.
 */
void log(const std::string& text);

#endif //MIGHTYMOUSEAI_H
