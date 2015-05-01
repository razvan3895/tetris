#include <stdio.h>
#include <iostream>
#include "Map.h"
#include <string>
using namespace std;

class Game {
private:
    Map left, right;
public:
    Game(int height, int width):left(height, width), right(height, width) { }
    void initGame() {
        left.startNextIteration();
        right.startNextIteration();
    }
    void moveUpA() {
        left.moveUp();
    }
    void moveUpB() {
        right.moveUp();
    }
    void moveDownA() {
        left.moveDown();
    }
    void moveDownB() {
        right.moveDown();
    }
    void rotateA() {
        left.rotate();
    }
    void rotateB() {
        right.rotate();
    }
    void moveForwardA() {
        int score_before = left.score;
        left.moveForward();
        if(left.score > score_before)
            right.decreaseWidth();
    }
    void moveForwardB() {
        int score_before = right.score;
        right.moveForward();
        if(right.score > score_before)
            left.decreaseWidth();
    }

    bool gameOver() {
        return left.isFull || right.isFull;
    }

    void represent() {
        for(int i = 0 ; i < left.height; ++i) {
            for(int j = 0; j < left.width; ++j)
                switch(left.tiles[i][j]) {
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
            cout << " || ";
           
            for(int j = right.width - 1; j >= 0; --j)
                switch(right.tiles[i][j]) {
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
            cout << '\n';
            }
        std::cout << '\n';
    }
};

int main() {
    string op;
    Game g(7, 10);
    g.initGame();
    while(!g.gameOver()) {
        g.represent();
        cin >> op;
        if(op == "w")
            g.moveUpA();
        if(op == "s")
            g.moveDownA();
        if(op == "a")
            g.rotateA();
        if(op == "d")
            g.moveForwardA();
        if(op == "u")
            g.moveUpB();
        if(op == "j")
            g.moveDownB();
        if(op == "k")
            g.rotateB();
        if(op == "h")
            g.moveForwardB();

        // if(op == "chk")
            // cout << map.columnIsFull(9);
    }
    return 0;
}