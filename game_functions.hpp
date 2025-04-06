#ifndef GAME_FUNCTIONS
#define GAME_FUNCTIONS

#include <vector>
#include <string>
#include <SDL3/SDL.h>
using namespace std;

// global variables to customize look
extern int squareSize;
extern int spaceBetweenSquares;
extern int buttonArea;

// Definition for the Unit struct
struct Unit {
    SDL_FRect* square;
    bool active;

    Unit(int x, int y, int h, int w) {
        square = new SDL_FRect();
        square->x = x; square ->y = y; square->h = h; square->w = w;
        active = false;
    }

    void checkPress(int x, int y) {
        if (x >= square->x &&
            x <= square->x + (squareSize + spaceBetweenSquares) &&
            y >= square->y &&
            y <= square->y + (squareSize + spaceBetweenSquares))
            active = !active;
    }

};

// Definition for the Button struct
struct Button {
    SDL_FRect* square;
    bool pressed;

    Button(int x, int y, int h, int w) {
        square = new SDL_FRect();
        square->x = x; square->y = y; square->h = h; square->w = w;
        pressed = false;
    }

    bool checkPress(int x, int y) {
        if (x >= square->x &&
            x <= square->x + square->w &&
            y >= square->y &&
            y <= square->y + square->h)
            return true;
        else
            return false;
    }
};


// Function prototypes
int checkSurroundings(Unit** space[], int verticalSize, int horizontalSize, int row, int column);
void nextGeneration(Unit** space[], int verticalSize, int horizontalSize);
void printArea(vector<vector<int>> area);

#endif