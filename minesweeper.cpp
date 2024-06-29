#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
/***************************************************MINESWEEPER*************************************************/
#define SIZE 10
#define MINES 10

typedef struct {
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int neighborMines;
} Cell;

Cell board[SIZE][SIZE];
int remainingFlags;

// Function to initialize the game board
void initializeBoard() {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j].isMine = false;
            board[i][j].isRevealed = false;
            board[i][j].isFlagged = false;
            board[i][j].neighborMines = 0;
        }
    }
}

// Function to randomly place mines on the game board
void plantMines() {
    int count = 0;
    while (count < MINES) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        if (!board[row][col].isMine) {
            board[row][col].isMine = true;
            count++;
        }
    }
}

// Function to calculate the number of neighboring mines for each cell
void calculateNeighborMines() {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (!board[i][j].isMine) {
                int count = 0;
                // Calculating the number of mines in all the 8 adjacent cells
                if (i > 0 && j > 0 && board[i - 1][j - 1].isMine) count++;
                if (i > 0 && board[i - 1][j].isMine) count++;
                if (i > 0 && j < SIZE - 1 && board[i - 1][j + 1].isMine) count++;
                if (j > 0 && board[i][j - 1].isMine) count++;
                if (j < SIZE - 1 && board[i][j + 1].isMine) count++;
                if (i < SIZE - 1 && j > 0 && board[i + 1][j - 1].isMine) count++;
                if (i < SIZE - 1 && board[i + 1][j].isMine) count++;
                if (i < SIZE - 1 && j < SIZE - 1 && board[i + 1][j + 1].isMine) count++;
                board[i][j].neighborMines = count;
            }
        }
    }
}

// Function to reveal empty cells recursively
void revealEmptyCells(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col].isRevealed) {
        return;
    }

    board[row][col].isRevealed = true;

    if (board[row][col].neighborMines == 0) {
        // Reveal neighboring cells recursively
        revealEmptyCells(row - 1, col - 1);
        revealEmptyCells(row - 1, col);
        revealEmptyCells(row - 1, col + 1);
        revealEmptyCells(row, col - 1);
        revealEmptyCells(row, col + 1);
        revealEmptyCells(row + 1, col - 1);
        revealEmptyCells(row + 1, col);
        revealEmptyCells(row + 1, col + 1);
    }
}

// Function to print the game board
void printBoard() {
    int i, j;
    printf("\n   ");
    for (i = 0; i < SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n\n");
    for (i = 0; i < SIZE; i++) {
        printf("%d  ", i);
        for (j = 0; j < SIZE; j++) {
            if (!board[i][j].isRevealed) {
                if (board[i][j].isFlagged) {
                    printf("F ");
                } else {
                    printf(". ");
                }
            } else if (board[i][j].neighborMines > 0) {
                printf("%d ", board[i][j].neighborMines);
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
    printf("\nRemaining flags: %d\n", remainingFlags);
}

int main() {
    srand(time(NULL));
    printf("Welcome to Minesweeper!\n");
    printf("Instructions:\n");
    printf(" - The game is played on a grid of cells, some containing hidden mines.\n");
    printf(" - The objective is to reveal all cells without stepping on a mine.\n");
    printf(" - Enter your move in the format 'row col action' (e.g., '2 3 R' to reveal cell at row 2, column 3).\n");
    printf(" - Action 'R' is to reveal a cell, and 'F' is to flag/unflag a cell.\n");
    printf(" - The grid is labeled with row and column numbers for reference.\n");
    printf(" - When playing the game, the numbers displayed on the grid indicate the number of neighboring mines for each cell.\n");
    printf(" - The current state of the board will be displayed.\n");
    printf(" - Good luck and have fun!\n\n");

    initializeBoard();
    plantMines();
    calculateNeighborMines();
    remainingFlags = MINES + 1;

    int row, col;
    char action;

    while (true) {
        printBoard();

        printf("\nEnter your move (row col action [R - reveal, F - flag]): ");
        scanf("%d %d %c", &row, &col, &action);

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
            printf("Invalid move! Try again.\n");
            continue;
        }

        if (action == 'R' || action == 'r') {
            if (board[row][col].isMine) {
                printf("\nGame Over! You stepped on a mine.\n");
                break;
            }
            revealEmptyCells(row, col);
        } else if (action == 'F' || action == 'f') {
            if (board[row][col].isRevealed) {
                printf("\nInvalid move! You cannot flag a cell that has already been revealed.\n");
                continue;
            }
            if (board[row][col].isFlagged) {
                board[row][col].isFlagged = false;
                remainingFlags++;
            } else {
                if (remainingFlags == 0) {
                    printf("\nNo flags remaining! Cannot place any more flags.\n");
                    continue;
                }
                board[row][col].isFlagged = true;
                remainingFlags--;
            }
        } else {
            printf("Invalid action! Try again.\n");
            continue;
        }

        bool allMinesFlagged = true;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j].isMine && !board[i][j].isFlagged) {
                    allMinesFlagged = false;
                    break;
                }
            }
        }

        if (allMinesFlagged) {
            printf("\nCongratulations! You flagged all the mines. You win!\n");
            break;
        }
    }

    return 0;
}
