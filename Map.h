#include "MapObject.h"
#include <stdlib.h>

#define NUMBER_OF_SHAPES	7

class Map {
public:
	int **tiles;
	int height;
	int maxWidth;
	int width;
	float score;
	bool isFull;
	Shape shapes[NUMBER_OF_SHAPES];
	MapObject currentObject;
	MapObject nextObject;

	Map() {}
	Map(const int&, const int&);
	~Map();
	void increaseWidth();
	void decreaseWidth();
	bool columnIsEmpty(const int&);
	bool columnIsFull(const int&);
	void shiftRightOverColumn(const int&);
	void shiftLeftOverColumn(const int&);
	bool iterationIsOver();	
	void startNextIteration();
	bool drawCurrentObject();
	void clearCurrentObject();
	void initializeShapes();
	void represent();
	void moveForward();
	void rotate();
	void moveUp();
	void moveDown();
};