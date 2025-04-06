#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "game_functions.hpp"

using namespace std;

// Global Variables
int squareSize = 16;
int spaceBetweenSquares = 3;
int buttonArea = 100;
bool inLoop = false;

// Variables for setting FPS
const int FPS = 30;
const int frameDelay = 1000 / FPS;
Uint32 frameStart;
int frameTime;

// Setting the different colors
SDL_Color deactiveColor = {0, 0, 0, 255};
SDL_Color activeColor = {255, 255, 255, 255};
SDL_Color gray = {128, 128, 128, 255};

// Creating the 3 buttons
Button stepButton = Button(0, 0, 50, 100);
Button startButton = Button(0, 0, 50, 100);
Button clearButton = Button(0, 0, 50, 100);


// Function for updating the area if the window gets resized
void updateArea(Unit** oldArea[], Unit** newArea[], int oldVert, int oldHoriz, int newVert, int newHoriz, int newHorizontalPadding, int newVerticalPadding)
{
    // Setting the new spots for the horizontal and vertical axis
    int horizontalSpot = newHorizontalPadding;
    int verticalSpot = newVerticalPadding;

    // Loops through the new area, copying the units from the old area to the new one.
    // Creates new rows if needed
    for (int i = 0; i < newVert; i++) {
        for (int j = 0; j < newHoriz; j++) {
            if (j >= oldHoriz || i >= oldVert) {
                Unit* newUnit = new Unit(horizontalSpot, verticalSpot, squareSize, squareSize);
                newArea[i][j] = newUnit;
                horizontalSpot += squareSize + spaceBetweenSquares;
            }
            else {
                Unit* newUnit = new Unit(horizontalSpot, verticalSpot, squareSize, squareSize);
                newUnit->active = oldArea[i][j]->active;
                newArea[i][j] = newUnit;
                horizontalSpot += squareSize + spaceBetweenSquares;
            }
        }
        horizontalSpot = newHorizontalPadding;
        verticalSpot += squareSize + spaceBetweenSquares;
    }

}


// Function for clearing the area if the user presses the clear button
void clearArea(Unit** area[], int verticalSize, int horizontalSize)
{
    for (int i = 0; i < verticalSize; i++) {
        for (int j = 0; j < horizontalSize; j++) {
            area[i][j]->active = false;
        }
    }
}


// Function renders the current frame given the area and correct colors
void renderScreen(SDL_Window* window, SDL_Renderer* renderer, Unit** area[], int numForVertical, int numForHorizontal)
{
    SDL_SetRenderDrawColor(renderer, gray.r, gray.g, gray.b, gray.a);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, deactiveColor.r, deactiveColor.g, deactiveColor.b, deactiveColor.a);

    for (int i = 0; i < numForVertical; i++) {
        for (int j = 0; j < numForHorizontal; j++) {
            if (area[i][j]->active)
                SDL_SetRenderDrawColor(renderer, activeColor.r, activeColor.g, activeColor.b, activeColor.a);
            else
                SDL_SetRenderDrawColor(renderer, deactiveColor.r, deactiveColor.g, deactiveColor.b, deactiveColor.a);

            SDL_RenderFillRect(renderer, area[i][j]->square);
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, stepButton.square);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, startButton.square);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, clearButton.square);

    SDL_RenderPresent(renderer);
}



// Command line parameters must be there for the main function to be recognized by SDL
int main(int argc, char** args) {

    // Initialize SDL
	SDL_Init(SDL_INIT_VIDEO||SDL_INIT_EVENTS||SDL_INIT_CAMERA);

    // Pointers to window and renderer
	SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    // Creates window and renderer
    window = SDL_CreateWindow("Conway Game of Life", 1280, 720, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, NULL);


    // Setting variables for the window size
    int windowWidth;
    int windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);


    // Calculating the number of units for the horizontal and vertical
    int numForHorizontal = windowWidth / (squareSize+spaceBetweenSquares);
    int numForVertical = (windowHeight - buttonArea) / (squareSize+spaceBetweenSquares);


    // Calculating padding for the edges of the screen
    int horizontalPadding = (windowWidth - ((numForHorizontal * (squareSize + spaceBetweenSquares)) - spaceBetweenSquares)) / 2;
    // int verticalPadding = (windowHeight - (((windowHeight / (squareSize+spaceBetweenSquares)) * (squareSize + spaceBetweenSquares)) - spaceBetweenSquares)) / 2;
    int verticalPadding = horizontalPadding;
    
    // Creating the area and filling it with units
    Unit*** area = new Unit**[numForVertical];
    for (int i = 0; i < numForVertical; i++) {
        area[i] = new Unit*[numForHorizontal];
        for (int j = 0; j < numForHorizontal; j++)
            area[i][j] = nullptr;
    }

    // Debug print
    cout << "Filling up area with Units" << endl;

    // Filling the area with empty Units
    int horizontalSpot = horizontalPadding;
    int verticalSpot = verticalPadding;
    for (int i = 0; i < numForVertical; i++) {
        for (int j = 0; j < numForHorizontal; j++) {
            Unit* newUnit = new Unit(horizontalSpot, verticalSpot, squareSize, squareSize);
            area[i][j] = newUnit;
            horizontalSpot += (squareSize+spaceBetweenSquares);
        }
        horizontalSpot = horizontalPadding;
        verticalSpot += (squareSize+spaceBetweenSquares);
    }

    // Debug print
    cout << "Area filled" << endl;


    // Calculating and setting positions for the 3 buttons
    int buttonPosY = windowHeight - verticalPadding - 50 - ((buttonArea - 50) / 2);
    int stepButtonX = (windowWidth / 4) - (50);
    int startButtonX = (windowWidth / 2) - (50/2);
    int clearButtonX = windowWidth - (windowWidth / 4) - (50/2);

    stepButton.square->x = stepButtonX; stepButton.square->y = buttonPosY;
    startButton.square->x = startButtonX; startButton.square->y = buttonPosY;
    clearButton.square->x = clearButtonX; clearButton.square->y = buttonPosY;



    //
    // Main Loop
    //
    bool quit = false;
    SDL_Event event;
    while (!quit) {

        frameStart = SDL_GetTicks();

        // Loop checks conditions for different events.
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_EVENT_QUIT)
                quit = true;

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                // Storing the position of the click
                float posx = event.button.x;
                float posy = event.button.y;

                // Finding out what unit the click was targeting
                int unitx = (posx - horizontalPadding) / (squareSize+spaceBetweenSquares);
                int unity = (posy - verticalPadding) / (squareSize+spaceBetweenSquares);

                // Logging output
                cout << "Pos x: " << posx << endl << "Pos y: " << posy << endl;
                cout << "Unit x: " << unitx << endl << "Unit y: " << unity << endl << endl;

                
                // If the click was inside the playable area bounds, switch the square
                if (posx <= windowWidth - horizontalPadding && posx >= horizontalPadding &&
                    posy <= windowHeight - buttonArea && posy >= verticalPadding)
                {
                    cout << "checking within area bounds" << endl;
                    inLoop = false;
                    area[unity][unitx]->checkPress(posx, posy);
                    continue;
                }

                // If clicked within the button area, check which button was pressed
                if (posy >= windowHeight - buttonArea) {
                    cout << "Checking within button area" << endl;
                    if (stepButton.checkPress(posx, posy)) {
                        cout << "clicked within stepButton area" << endl;
                        nextGeneration(area, numForVertical, numForHorizontal);
                        inLoop = false;
                        continue;
                    }
                    if (startButton.checkPress(posx, posy)) {
                        cout << "Clicked within startButton area" << endl;
                        inLoop = !inLoop;
                        continue;
                    }
                    if (clearButton.checkPress(posx, posy)) {
                        cout << "Clicked within clearButton area. Clearing area." << endl;
                        clearArea(area, numForVertical, numForHorizontal);
                        inLoop = false;
                        continue;
                    }
                }
            }

            if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                // Get the new size of the window
                SDL_GetWindowSize(window, &windowWidth, &windowHeight);

                // Finding the new amount of units in each row
                int oldNumForVertical = numForVertical;
                int oldNumForHorizontal = numForHorizontal;
                numForHorizontal = windowWidth / (squareSize+spaceBetweenSquares);
                numForVertical = (windowHeight - buttonArea) / (squareSize+spaceBetweenSquares);

                // Calculating padding for the edges of the screen
                horizontalPadding = (windowWidth - ((numForHorizontal * (squareSize + spaceBetweenSquares)) - spaceBetweenSquares)) / 2;
                // verticalPadding = (windowHeight - (((windowHeight / (squareSize + spaceBetweenSquares)) * (squareSize + spaceBetweenSquares)) - spaceBetweenSquares)) / 2;
                verticalPadding = horizontalPadding;

                // Creates a new area with the new sizes
                cout << "Creating new area with new size" << endl;
                Unit*** newArea = new Unit**[numForVertical];
                for (int i = 0; i < numForVertical; i++) {
                    newArea[i] = new Unit*[numForHorizontal];
                }

                // Updates the new area with the units from the old area
                updateArea(area, newArea, oldNumForVertical, oldNumForHorizontal, numForVertical, numForHorizontal, horizontalPadding, verticalPadding);

                // Swapping the area pointer to point to the new area
                Unit*** temp = area; 
                area = newArea;

                // Deletes old area (unsure if it works currently)
                cout << "Deleting old area" << endl;
                delete[] temp;
                temp = nullptr;

                // Calculating and setting new positions for the buttons
                buttonPosY = windowHeight - verticalPadding - 50 - ((buttonArea - 50) / 2);
                stepButtonX = (windowWidth / 4) - (50);
                startButtonX = (windowWidth / 2) - (50/2);
                clearButtonX = windowWidth - (windowWidth / 4) - (50/2);

                stepButton.square->x = stepButtonX; stepButton.square->y = buttonPosY;
                startButton.square->x = startButtonX; startButton.square->y = buttonPosY;
                clearButton.square->x = clearButtonX; clearButton.square->y = buttonPosY;
            }
        }

        // If the user pressed the startButton, it will keep playing in a loop with a delay between generations
        if (inLoop) {
            nextGeneration(area, numForVertical, numForHorizontal);
            SDL_Delay(250);
        }

        // Renders screen
        renderScreen(window, renderer, area, numForVertical, numForHorizontal);

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);

    }


    // Exiting functions
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
	
	return 0;
}


/*  
    Compiler command:
    g++ -o GameOfLife game.cpp game_functions.cpp -LC:\MinGW\lib -lSDL3 -mwindows
*/