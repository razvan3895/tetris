#ifndef	_MAP_OBJECT_H_
#define _MAP_OBJECT_H_

#define MAX_NUMBER_OF_TILES		5
#define MAX_NUMBER_OF_STATES	4

#define EMPTY	0
#define CYAN    1
#define BLUE    2
#define ORANGE  3
#define YELLOW  4
#define GREEN   5
#define PURPLE  6
#define RED     7

struct Point {
	int x;
	int y;
};

struct Shape {	
	int size;
	int number_of_states;	
	int color;
	Point relations_to_center[MAX_NUMBER_OF_STATES][MAX_NUMBER_OF_TILES];

	Shape();
	Shape(const int&, const int&, const int&,
		Point[MAX_NUMBER_OF_STATES][MAX_NUMBER_OF_TILES]);
	Shape(const Shape &other);
	void operator=(const Shape& other);
};

struct MapObject: public Shape {
	int state;
	Point coordinates[MAX_NUMBER_OF_TILES];

	MapObject();
	MapObject(const Shape&, const int&);
	MapObject(const MapObject&);	
	void operator=(const MapObject&);
	bool rotateClockwise(const int&, const int&);
	bool rotateAntiClockwise(const int&, const int&);
	bool moveUp(const int&);
	bool moveDown(const int&);
	bool moveForward(const int&);
	void setCoordinates();
	bool invalidCoordinates(const int&, const int&);
	void printCoordinates();	
	void draw();
};

#endif