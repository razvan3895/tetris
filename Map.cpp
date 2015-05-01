#include "Map.h"
#include <iostream>
#include <time.h>

Map::Map(const int &hght, const int &wdth) {
	score = 0;
	height = hght;
	width = wdth;
	maxWidth = 2 * wdth;
	tiles = new int*[height];
	for(int i = 0; i < height; ++i) {
		tiles[i] = new int[maxWidth];
		for(int j = 0; j < maxWidth; ++j)
			tiles[i][j] = EMPTY;
	}
	initializeShapes();
	srand(time(NULL));
	nextObject = MapObject(shapes[rand() % NUMBER_OF_SHAPES], height);
	isFull = 0;
}

Map::~Map() {
	for(int i = 0; i < height; ++i)
		delete[] tiles[i];
	delete[] tiles;
}

void Map::increaseWidth() {
	++width;
	shiftRightOverColumn(width - 1);
}

void Map::decreaseWidth() {
	--width;
	shiftLeftOverColumn(0);
}
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
    // Line
    Point relations6[MAX_NUMBER_OF_STATES][MAX_NUMBER_OF_TILES] =
        {{{0, -1}, {0, 1}, {0, 2}}, {{-1, 0}, {1, 0}, {2, 0}}};
    shapes[6] = Shape(4, 2, CYAN, relations6);
}

bool Map::columnIsEmpty(const int &index) {
	for(int i = 0; i < height; ++i)
		if(tiles[i][index] != EMPTY) {
			return false;
		}
	return true;
}

bool Map::columnIsFull(const int &index) {
	for(int i = 0; i < height; ++i)
		if(tiles[i][index] == EMPTY)
			return false;
	return true;
}

void Map::shiftRightOverColumn(const int &index) {
	for(int i = index; i >= 0; --i) {
		for(int j = 0; j < height; ++j)
			tiles[j][i] = tiles[j][i - 1];
	}
}

void Map::shiftLeftOverColumn(const int &index) {
	for(int i = index; i <= width ; ++i) {
		for(int j = 0; j < height; ++j)
			tiles[j][i] = tiles[j][i + 1];
	}
}

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

void Map::clearCurrentObject() {
	for(int i = 0; i < currentObject.size; ++i)
		tiles[currentObject.coordinates[i].y][currentObject.coordinates[i].x] = EMPTY;
}

bool Map::iterationIsOver() {
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

void Map::startNextIteration() {
	for(int i = width - 1; i >= 0; --i)
		if(columnIsEmpty(i)) {
			break;
		}
		else while(columnIsFull(i)) {
			++score;
			shiftRightOverColumn(i);
			decreaseWidth();
		}

	currentObject = nextObject;
	nextObject = MapObject(shapes[rand() % NUMBER_OF_SHAPES], height);
	isFull = !drawCurrentObject();
}

void Map::moveForward() {
	// std::cout << "WOOP" << '\n';
	clearCurrentObject();
	bool isMoved = currentObject.moveForward(width);
	drawCurrentObject();
	if(iterationIsOver()) {
		startNextIteration();
	}
}

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

void Map::represent() {
	for(int i = 0 ; i < height; ++i) {
		for(int j = 0; j < width; ++j)
			switch(tiles[i][j]) {
				case EMPTY:
					std::cout << "* ";
					break;
				case RED:
					std::cout << "R ";
					break;
				case BLUE:
					std::cout << "B ";
					break;
				case CYAN:
					std::cout << "C ";
					break;
				case ORANGE:
					std::cout << "O ";
					break;
				case YELLOW:
					std::cout << "Y ";
					break;
				case GREEN:
					std::cout << "G ";
					break;
				case PURPLE:
					std::cout << "P ";
					break;
			}
		std::cout << '\n';
	}
	std::cout << "\t\t" << score << '\n';
}