#include "Map.h"
#include <iostream>
#include <time.h>


// A passive tetris arena
Map::Map(const int &hght, const int &wdth) {
    score = 0;
    height = hght;
    width = wdth;
    // Each map has a starting width, but also must be extendable to double
    // this initial value
    maxWidth = 2 * wdth;
    tiles = new int*[height];
    for(int i = 0; i < height; ++i) {
        tiles[i] = new int[maxWidth];
        for(int j = 0; j < maxWidth; ++j)
            tiles[i][j] = EMPTY;
    }
    initializeShapes();
    // The new shapes are generated based on pseudo-random numbers
    srand(time(NULL));
    nextObject = MapObject(shapes[rand() % NUMBER_OF_SHAPES], height);
    isFull = 0;
}

Map::~Map() {
    for(int i = 0; i < height; ++i)
        delete[] tiles[i];
    delete[] tiles;
}

// Increase width by shifting elements to the right
void Map::increaseWidth(bool isEndOfCycle = 1) {
    ++width;
    shiftRightOverColumn(width - 1);
}

// Decrease width by shifting elements to the left
void Map::decreaseWidth(bool isEndOfCycle = 1) {
    clearCurrentObject();
    --width;
    shiftLeftOverColumn(0);
    moveForward();
}

// Defines the shapes that a map can use

// I chose not to store all the possible states of a shape, because
// implementing a rotation matrix would be difficult on certain shapes (for
// example the square, which has no center Tile)
void Map::initializeShapes() {
    // Square
    Point relations0[MAX_NUMBER_OF_STATES][MAX_NUMBER_OF_TILES] =
        {{{0, 1}, {1, 1}, {1, 0}}};
    shapes[0] = Shape(4, 1, YELLOW, relations0);
    // Z
    Point relations1[MAX_NUMBER_OF_STATES][MAX_NUMBER_OF_TILES] =
        {{{0, 1}, {1, 0}, {1, -1}}, {{-1, 0}, {0, 1}, {1, 1}},
         {{0, -1}, {-1, 0}, {-1, 1}}, {{-1, -1}, {0, -1}, {1, 0}}};
    shapes[1] = Shape(4, 4, RED, relations1);
    // Mirror Z
    Point relations2[MAX_NUMBER_OF_STATES][MAX_NUMBER_OF_TILES] = 
        {{{0, -1}, {1, 0}, {1, 1}}, {{-1, 1}, {0, 1}, {1, 0}},
         {{-1, -1}, {-1, 0}, {0, 1}}, {{-1, 0}, {0, -1}, {1, -1}}};
    shapes[2] = Shape(4, 4, GREEN, relations2);
    // L
    Point relations3[MAX_NUMBER_OF_STATES][MAX_NUMBER_OF_TILES] =
        {{{0, -1}, {0, -2}, {1, 0}}, {{1, 0}, {2, 0}, {0, 1}},
         {{0, 1}, {0, 2}, {-1, 0}}, {{-1, 0}, {-2, 0}, {0, -1}}};
    shapes[3] = Shape(4, 4, ORANGE, relations3);
    // Mirror L
    Point relations4[MAX_NUMBER_OF_STATES][MAX_NUMBER_OF_TILES] =
        {{{1, 0}, {0, 1}, {0, 2}}, {{0, 1}, {-1, 0}, {-2, 0}},
         {{-1, 0}, {0, -1}, {0, -2}}, {{0, -1}, {1, 0}, {2, 0}}};
    shapes[4] = Shape(4, 4, BLUE, relations4);
    // T
    Point relations5[MAX_NUMBER_OF_STATES][MAX_NUMBER_OF_TILES] =
        {{{0, -1}, {1, 0}, {0, 1}}, {{1, 0}, {0, 1}, {-1, 0}},
         {{0, 1}, {-1, 0}, {0, -1}}, {{-1, 0}, {0, -1}, {1, 0}}};
    shapes[5] = Shape(4, 4, PURPLE, relations5);
    // Bar
    Point relations6[MAX_NUMBER_OF_STATES][MAX_NUMBER_OF_TILES] =
        {{{0, -1}, {0, 1}, {0, 2}}, {{-1, 0}, {1, 0}, {2, 0}}};
    shapes[6] = Shape(4, 2, CYAN, relations6);
}

// Tests to see if a column is empty
bool Map::columnIsEmpty(const int &index) {
    for(int i = 0; i < height; ++i)
        if(tiles[i][index] != EMPTY) {
            return false;
        }
    return true;
}

// Tests to see if a column is full, so it can be emptied
bool Map::columnIsFull(const int &index) {
    for(int i = 0; i < height; ++i)
        if(tiles[i][index] == EMPTY)
            return false;
    return true;
}

// Shifts matrix to the right, over the column "index"
void Map::shiftRightOverColumn(const int &index) {
    for(int i = index; i >= 0; --i) {
        for(int j = 0; j < height; ++j)
            tiles[j][i] = tiles[j][i - 1];
    }
}

// Shifts matrix to the left, over the column "index"
void Map::shiftLeftOverColumn(const int &index) {
    for(int i = index; i <= width ; ++i) {
        for(int j = 0; j < height; ++j)
            tiles[j][i] = tiles[j][i + 1];
    }
}

// Tests if the active object can be drawn into the tile matrix, and draws it
// if possible
bool Map::drawCurrentObject() {
    for(int i = 0; i < currentObject.size; ++i) {
        if(tiles[currentObject.coordinates[i].y][currentObject.coordinates[i].x] != EMPTY) {
            return false;
        }
    }
    for(int i = 0; i < currentObject.size; ++i)     
        tiles[currentObject.coordinates[i].y][currentObject.coordinates[i].x] = currentObject.color;
    return true;
}

// Clears the current object from the tile matrix
void Map::clearCurrentObject() {
    for(int i = 0; i < currentObject.size; ++i)
        tiles[currentObject.coordinates[i].y][currentObject.coordinates[i].x] = EMPTY;
}

// Checks whether a cycle has ended or not
bool Map::cycleIsOver() {
    int max_x[50];
    for(int i = 0; i < height; ++i)
        max_x[i] = 0;
    for(int i = 0; i < currentObject.size; ++i) {
        // Check if the current object is at the end of the arena
        if(currentObject.coordinates[i].x == width - 1) {
            return true;
        }
        // Find the furthest tile(s) width-wise
        if(currentObject.coordinates[i].x > max_x[currentObject.coordinates[i].y])
            max_x[currentObject.coordinates[i].y] = currentObject.coordinates[i].x;
    }
    // Check if there is room for one more movement
    for(int i = 0; i < height; ++i)
        if(tiles[i][max_x[i] + 1] != EMPTY) {
            return true;
        }
            
    return false;
}

// Starts a new cycle (gives a new active shape)
void Map::startNextCycle() {
    for(int i = width - 1; i >= 0; --i)
        if(columnIsEmpty(i)) {
            break;
        }
        else while(columnIsFull(i)) {
            ++score;
            shiftRightOverColumn(i);
            increaseWidth();
        }

    currentObject = nextObject;
    nextObject = MapObject(shapes[rand() % NUMBER_OF_SHAPES], height);
    isFull = !drawCurrentObject();
}

// Move the active shape forward
// If the move ends the cycle, start a new one
void Map::moveForward() {
    // std::cout << "WOOP" << '\n';
    clearCurrentObject();
    currentObject.moveForward(width);
    drawCurrentObject();
    if(cycleIsOver()) {
        startNextCycle();
    }
}

// Rotate the current shape, if possible
void Map::rotate() {
    MapObject previousObject = currentObject;
    clearCurrentObject();
    if(currentObject.rotateClockwise(height, width))
        if(!drawCurrentObject()) {
            currentObject = previousObject;
            drawCurrentObject();
        }
        else {
            // All good
        }
    else {
        drawCurrentObject();
    }
}

// Move the current shape up, if possible
void Map::moveUp() {
    clearCurrentObject();
    if(currentObject.moveUp(height)) {
        if(!drawCurrentObject()) {
            currentObject.moveDown(height);
            drawCurrentObject();
        }
    }
    else {
        drawCurrentObject();
    }
}

// Move the current shape down, if possible
void Map::moveDown() {
    clearCurrentObject();
    if(currentObject.moveDown(height)) {
        if(!drawCurrentObject()) {
            currentObject.moveUp(height);
            drawCurrentObject();
        }
    }
    else {
        drawCurrentObject();
    }
}
