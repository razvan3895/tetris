#include "MapObject.h"
#include <iostream>

Shape::Shape() {
	size = 0;
	number_of_states = 0;
	color = 0;
}

Shape::Shape(const int &sz, const int &no_of_states, const int &col,
	Point relations[MAX_NUMBER_OF_STATES][MAX_NUMBER_OF_TILES]) 
{
	size = sz;
	number_of_states = no_of_states;
	color = col;

	// For each Shape state, size - 1 relations are stored
	for(int i = 0; i < number_of_states; ++i)
		for(int j = 0; j < size - 1; ++j) {
			relations_to_center[i][j] = relations[i][j];
		}
}

Shape::Shape(const Shape& other) {
	size = other.size;
	number_of_states = other.number_of_states;
	color = other.color;

	for(int i = 0; i < number_of_states; ++i)
		for(int j = 0; j < size - 1; ++j)
			relations_to_center[i][j] = other.relations_to_center[i][j];	
}

void Shape::operator=(const Shape& other) {
	size = other.size;
	number_of_states = other.number_of_states;
	color = other.color;

	for(int i = 0; i < number_of_states; ++i)
		for(int j = 0; j < size - 1; ++j)
			relations_to_center[i][j] = other.relations_to_center[i][j];
}

MapObject::MapObject() {
	state = 0;
	size = 0;
	color = EMPTY;
	number_of_states = 0;
}

MapObject::MapObject(const Shape& shape, const int &height) {
	state = 0;
	size = shape.size;
	color = shape.color;
	number_of_states = shape.number_of_states;
	int min_x = 0;

	for(int i = 0; i < number_of_states; ++i)
		for(int j = 0; j < size - 1; ++j) {
			relations_to_center[i][j] = shape.relations_to_center[i][j];
		}

	for(int i = 0; i < size - 1; ++i)
		if(relations_to_center[state][i].x < min_x)
			min_x = relations_to_center[state][i].x;
	// std::cout << min_x << "****\n";
	coordinates[0].x = - min_x;
	coordinates[0].y = height / 2;

	setCoordinates();
}

MapObject::MapObject(const MapObject& other) {
	state = 0;
	size = other.size;
	color = other.color;
	number_of_states = other.number_of_states;

	for(int i = 0; i < number_of_states; ++i)
		for(int j = 0; j < size - 1; ++j) {
			relations_to_center[i][j] = other.relations_to_center[i][j];
		}

	for(int i = 0; i < size; ++i)
		coordinates[i] = other.coordinates[i];
}

void MapObject::operator=(const MapObject& other) {
	state = 0;
	size = other.size;
	color = other.color;
	number_of_states = other.number_of_states;

	for(int i = 0; i < number_of_states; ++i)
		for(int j = 0; j < size - 1; ++j) {
			relations_to_center[i][j] = other.relations_to_center[i][j];
		}

	for(int i = 0; i < size; ++i)
		coordinates[i] = other.coordinates[i];
}

void MapObject::setCoordinates() {
	// std::cout << coordinates[0].x << ' ' << coordinates[0].y << '\n';
	// for(int j = 0; j < size - 1; ++j)
	// 	std::cout << relations_to_center[state][j].x << ' ' << relations_to_center[state][j].y << '\n';
	// std::cout << '\n';
	for(int i = 1; i < size; ++i) {
		coordinates[i].x = coordinates[0].x + relations_to_center[state][i - 1].x;
		coordinates[i].y = coordinates[0].y + relations_to_center[state][i - 1].y;
	}
}

bool MapObject::invalidCoordinates(const int &height, const int &width) {
	for(int i = 0; i < size; ++i) {
		if(coordinates[i].x >= width || coordinates[i].y >= height
			|| coordinates[i].x < 0 || coordinates[i].y < 0)
			return true;
	}
	return false;
}

bool MapObject::rotateClockwise(const int& height, const int& width) {
	state = state + 1 >= number_of_states ? 0 : state + 1;
	setCoordinates();

	if(invalidCoordinates(height, width)) {
		rotateAntiClockwise(height, width);
		return false;
	}

	return true;
}

bool MapObject::rotateAntiClockwise(const int& height, const int& width) {
	state = state - 1 < 0 ? number_of_states - 1 : state - 1;
	setCoordinates();

	if(invalidCoordinates(height, width)) {
		rotateClockwise(height, width);
		return false;
	}

	return true;
}

bool MapObject::moveDown(const int& height) {
	for(int i = 0; i < size; ++i) {
		if(coordinates[i].y >= height - 1) {
			for(int j = 0; j < i; ++j)
				--coordinates[j].y;
			return false;
		}
		++coordinates[i].y;
	}
	return true;
}

bool MapObject::moveUp(const int& height) {
	for(int i = 0; i < size; ++i) {
		if(coordinates[i].y < 1) {
			for(int j = 0; j < i; ++j)
				++coordinates[j].y;
			return false;
		}
		--coordinates[i].y;
	}
	return true;
}

bool MapObject::moveForward(const int& width) {
	for(int i = 0; i < size; ++i)
		if(coordinates[i].x >= width - 1)
			return false;
	for(int i = 0; i < size; ++i)
		++coordinates[i].x;
	return true;
}

void MapObject::printCoordinates() {
	for(int i = 0; i < size; ++i) {
		std::cout << '(' << coordinates[i].x << ", " << coordinates[i].y << ") "; 
	}
	std::cout << '\n';
}

void MapObject::draw() {
	char arena[5][5];
	for(int i = 0; i < 5; ++i)
		for(int j = 0; j < 5; ++j)
			arena[i][j] = '*';
	for(int i = 0; i < size; ++i)
		arena[coordinates[i].y][coordinates[i].x] = '#';
	for(int i = 0; i < 5; ++i) {
		for(int j = 0; j < 5; ++j)
			std::cout << arena[i][j] << ' ';
		std::cout << '\n';
	}
	std::cout << '\n';
}