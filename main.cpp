#include <GL/freeglut.h>    // Include the freeglut library for OpenGL
#include <vector>           // Include the vector library for std::vector
#include <cmath>            // Include the cmath library for math functions like sin, cos
#include <iostream>         // Include the iostream library for input/output

// Define constants for window and board size
const int windowWidth = 400;                // Width of the window
const int windowHeight = 400;               // Height of the window
const int boardSize = 3;                     // Size of the tic-tac-toe board
const float squareSize = windowWidth / boardSize; // Size of each square on the board

enum class Player { None, X, O };           // Enum to represent players

std::vector<std::vector<Player>> board(boardSize, std::vector<Player>(boardSize, Player::None)); // 2D vector to represent the board
Player currentPlayer = Player::X;           // Current player, starts with X
bool gameEnded = false;                      // Flag to indicate if the game has ended

bool checkEnd(Player player);               // Function prototype for checking game end condition

void drawBoard() {
    glClearColor(0.4, 0.3, 0.2, 1.0);        // Set the clear color for the window
    glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer

    // Draw vertical lines
    glColor3f(0.0, 0.0, 0.0);                // Set the color for the lines (black)
    glBegin(GL_LINES);                        // Begin drawing lines
    for (int i = 1; i < boardSize; i++) {     // Loop through vertical lines
        glVertex2f(i * squareSize, 0);        // Define start point of line
        glVertex2f(i * squareSize, windowHeight); // Define end point of line
    }
    glEnd();                                  // End drawing lines

    // Draw horizontal lines
    glBegin(GL_LINES);                        // Begin drawing lines
    for (int i = 1; i < boardSize; i++) {     // Loop through horizontal lines
        glVertex2f(0, i * squareSize);        // Define start point of line
        glVertex2f(windowWidth, i * squareSize); // Define end point of line
    }
    glEnd();                                  // End drawing lines

    // Draw Xs and Os
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] == Player::X) {
                // Draw X
                glColor3f(0.2, 0.5, 0.9);   // Set the color for X (blue)
                glBegin(GL_LINES);          // Begin drawing lines
                glVertex2f(j * squareSize + 0.1 * squareSize, i * squareSize + 0.1 * squareSize); // Top-left to bottom-right
                glVertex2f((j + 1) * squareSize - 0.1 * squareSize, (i + 1) * squareSize - 0.1 * squareSize); // Bottom-left to top-right
                glVertex2f((j + 1) * squareSize - 0.1 * squareSize, i * squareSize + 0.1 * squareSize); // Top-right to bottom-left
                glVertex2f(j * squareSize + 0.1 * squareSize, (i + 1) * squareSize - 0.1 * squareSize); // Bottom-right to top-left
                glEnd();                    // End drawing lines
            } else if (board[i][j] == Player::O) {
                // Draw O
                glColor3f(0.5, 0.8, 0.5);   // Set the color for O (green)
                glBegin(GL_LINE_LOOP);      // Begin drawing line loop
                for (int k = 0; k < 360; k += 10) { // Loop to draw circle
                    float x = j * squareSize + 0.5 * squareSize + 0.4 * squareSize * cos(k * 3.14159 / 180); // Calculate x coordinate
                    float y = i * squareSize + 0.5 * squareSize + 0.4 * squareSize * sin(k * 3.14159 / 180); // Calculate y coordinate
                    glVertex2f(x, y);       // Set the vertex
                }
                glEnd();                    // End drawing line loop
            }
        }
    }

    if (gameEnded) {
        std::string message;
        if (checkEnd(Player::X)) {
            message = "Player X wins!";
        } else if (checkEnd(Player::O)) {
            message = "Player O wins!";
        } else {
            message = "It's a draw!";
        }

        glColor3f(1.0, 1.0, 1.0);            // Set the color for text (white)
        glRasterPos2i(10, windowHeight - 20); // Set the position for text
        for (char c : message) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // Display the message
        }
    }

    glutSwapBuffers();                       // Swap the buffers
}

bool checkEnd(Player player) {
    // Check rows, columns, and diagonals for a win
    for (int i = 0; i < boardSize; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
            return true; // Row win
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
            return true; // Column win
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return true; // Diagonal win
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return true; // Diagonal win

    // Check for a draw
    bool boardFull = true;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] == Player::None) {
                boardFull = false; // Board is not full
                break;
            }
        }
        if (!boardFull) {
            break;
        }
    }
    return boardFull; // Game is a draw if the board is full and no player has won
}


void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !gameEnded) {
        int row = y / squareSize;
        int col = x / squareSize;

        if (board[row][col] == Player::None) {
            board[row][col] = currentPlayer;
            if (checkEnd(currentPlayer)) {
                gameEnded = true;
            } else {
                currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
            }
        }
    }

    glutPostRedisplay();                     // Post a redisplay event
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                    // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Set display mode
    glutInitWindowSize(windowWidth, windowHeight); // Set window size
    glutCreateWindow("Tic-Tac-Toe");          // Create window with title
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1); // Set orthographic projection
    glutDisplayFunc(drawBoard);               // Set display function
    glutMouseFunc(mouseClick);                // Set mouse function
    glutMainLoop();                           // Enter main loop

    return 0;                                // Return success
}
