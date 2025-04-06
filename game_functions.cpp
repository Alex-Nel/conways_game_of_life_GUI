#include <iostream>
#include <vector>
#include <string>
#include "game_functions.hpp"

using namespace std;


/*
Checks the surrounding tiles around a specific tile given a row and column
and returns the number of active tiles
*/
int checkSurroundings(Unit** space[], int verticalSize, int horizontalSize, int row, int column)
{
    int surrounding = 0;
    int newRow = row;
    int newColumn = column;

    newRow++;
    newColumn++;
    while (newRow > row-2 && newRow >= 0) {
        while (newColumn > column-2 && newColumn >= 0) {
            if (newRow >= verticalSize)
                break;
            if (newColumn >= horizontalSize) {
                newColumn--;
                continue;
            }

            if (space[newRow][newColumn]->active == true)
                surrounding++;

            newColumn--;
        }
        newRow--;
        newColumn = column+1;
    }

    if (space[row][column]->active == true) surrounding--;

    return surrounding;
}




// Finds the next Generation (or 'step') given a space vector, and returns that vector
void nextGeneration(Unit** space[], int verticalSize, int horizontalSize)
{
    cout << "Making next generation" << endl;
    vector<vector<int>> nextSpace(verticalSize);
    int surroundingActive = 0;

    for (int i = 0; i < verticalSize; i++) {
        // Goes through each row
        for (int j = 0; j < horizontalSize; j++) {
            // Calls surroundingActive function to check the number of active Units surrounding it
            surroundingActive = checkSurroundings(space, verticalSize, horizontalSize, i, j);

            // Branch checks the Unit with the matching conditions depending on if it's active or not
            // Adds a 0 to nextSpace if it'll deactivate, or a 1 if it'll be active
            if (space[i][j]->active == true) {
                if (surroundingActive < 2) {
                    nextSpace[i].push_back(0);
                    continue;
                }
                if (surroundingActive > 3) {
                    nextSpace[i].push_back(0);
                    continue;
                }

                nextSpace[i].push_back(1);
            }
            else if (space[i][j]->active == false) {
                if (surroundingActive == 3) {
                    nextSpace[i].push_back(1);
                    continue;
                }

                nextSpace[i].push_back(0);
            }
        }
    }

    // printArea(nextSpace);

    for (int i = 0; i < verticalSize; i++) {
        for (int j = 0; j < horizontalSize; j++) {
            space[i][j]->active = nextSpace[i][j];
        }
    }
}




// Prints the area. 0's are printed as white square, and 1's are printed as black square
// Used for debugging areas made with vectors
void printArea(vector<vector<int>> area)
{
    for (int i = 0; i < area.size(); i++) {
        for (int i : area[i]) {
            // cout << i << " ";
            if (i == 1)
                cout << "1 ";
                //wcout << L"\x25A0 ";
            if (i == 0)
                cout << "0 ";
                //wcout << L"\x25A1 ";
        }
        cout << endl;
        //wcout << endl;
    }
    cout << endl;
    // wcout << endl;
}