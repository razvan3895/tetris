#ifndef _GAME_H_
#define _GAME_H_

#include "Map.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define TILE_SIZE   16

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;


// Turns a color macro into a RGB sequence
void numberToRGB(int code, int rgb[3]) {
    switch(code) {
        case RED:
            rgb[0] = 100, rgb[1] = 0, rgb[2] = 0;
            break;
        case BLUE:
            rgb[0] = 0, rgb[1] = 0, rgb[2] = 100;
            break;
        case CYAN:
            rgb[0] = 0, rgb[1] = 255, rgb[2] = 255;
            break;
        case GREEN:
            rgb[0] = 0, rgb[1] = 255, rgb[2] = 0;
            break;
        case ORANGE:
            rgb[0] = 255, rgb[1] = 102, rgb[2] = 0;
            break;
        case PURPLE:
            rgb[0] = 255, rgb[1] = 0, rgb[2] = 255;
            break;
        case YELLOW:
            rgb[0] = 255, rgb[1] = 255, rgb[2] = 0;
            break; 
        case EMPTY:
            rgb[0] = 0, rgb[1] = 0, rgb[2] = 0;
            break;
    }
}

// The active instance of Map
class Game {
private:
    Map left, right;
public:
    Game(int height, int width):left(height, width), right(height, width) { }

    void initGame() {
        left.startNextCycle();
        right.startNextCycle();
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
            right.decreaseWidth(0);
    }

    void moveForwardB() {
        int score_before = right.score;
        right.moveForward();
        if(right.score > score_before)
            left.decreaseWidth(0);
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
            std::cout << " || ";
           
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
            std::cout << '\n';
            }
        std::cout << '\n';
    }

    void draw() {
        int rgb[3];
        al_clear_to_color(al_map_rgb(0,0,0));
        // Draw the outline of the arena
        al_draw_filled_rectangle(20, 20, 620, 40, al_map_rgb(224, 223, 219));
        al_draw_filled_rectangle(20, 40,
            40, 40 + left.height * TILE_SIZE, al_map_rgb(224, 223, 219));
        al_draw_filled_rectangle(600, 20, 620, 
            40 + left.height * TILE_SIZE, al_map_rgb(224, 223, 219));
        al_draw_filled_rectangle(20, 40 + left.height * TILE_SIZE,
            620, 60 + left.height * TILE_SIZE, al_map_rgb(224, 223, 219));
        al_draw_filled_rectangle(20, 40 + left.height * TILE_SIZE,
            620, 60 + left.height * TILE_SIZE, al_map_rgb(224, 223, 219));
        al_draw_filled_rectangle(40 + left.width * TILE_SIZE, 40 ,
            360 + (left.width - right.width) * TILE_SIZE * 2, 40 + left.height * TILE_SIZE,
            al_map_rgb(224, 223, 219));

        // Draw the arena itself
        for(int i = 0; i < left.height; ++i) {
            for(int j = 0; j < left.width; ++j) {
                numberToRGB(left.tiles[i][j], rgb);
                al_draw_filled_rectangle(40 + j * TILE_SIZE, 40 + i * TILE_SIZE, 40 + (j+1) * TILE_SIZE,
                    40 + (i+1) * TILE_SIZE, al_map_rgb(rgb[0], rgb[1], rgb[2]));

            }
            for(int j = 0; j < right.width; ++j) {                
                numberToRGB(right.tiles[i][j], rgb);
                al_draw_filled_rectangle(600 - (j+1) * TILE_SIZE, 40 + i * TILE_SIZE, 600 - j * TILE_SIZE,
                    40 + (i+1) * TILE_SIZE, al_map_rgb(rgb[0], rgb[1], rgb[2]));
            }

        }
        for(int i = 0; i < left.nextObject.size; ++i) {
            numberToRGB(left.nextObject.color, rgb);
            al_draw_filled_rectangle(
                20 + left.nextObject.coordinates[i].x * TILE_SIZE,                
                40 + (left.height + left.nextObject.coordinates[i].y) * TILE_SIZE,
                20 + (left.nextObject.coordinates[i].x + 1) * TILE_SIZE,                
                40 + (left.height + left.nextObject.coordinates[i].y + 1) * TILE_SIZE,
                al_map_rgb(rgb[0], rgb[1], rgb[2]));
        }

        for(int i = 0; i < right.nextObject.size; ++i) {
            numberToRGB(right.nextObject.color, rgb);
            al_draw_filled_rectangle(
                620 - right.nextObject.coordinates[i].x * TILE_SIZE,                
                40 + (right.height + right.nextObject.coordinates[i].y) * TILE_SIZE,
                620 - (right.nextObject.coordinates[i].x + 1) * TILE_SIZE,                
                40 + (right.height + right.nextObject.coordinates[i].y + 1) * TILE_SIZE,
                al_map_rgb(rgb[0], rgb[1], rgb[2]));
        }

        al_flip_display();
    }
};

#endif
