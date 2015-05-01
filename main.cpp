#include <stdio.h>
#include <iostream>
#include <string>
#include "Game.h"
using namespace std;



int main() {
    float ratio = 1;
    bool doExit = 0;

    if(!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    if(!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return -1;
    }

    display = al_create_display(640, 300);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        return -1;
    }

    timer = al_create_timer(ratio);
    if(!timer) {
            fprintf(stderr, "failed to create timer!\n");
            return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_start_timer(timer);
    al_flip_display();
    string op;
    Game g(8, 15);
    g.initGame();

    while(!g.gameOver() && !doExit) {
        g.draw();
        // Decomment the next line for real time console output
        // g.represent();
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if(ev.type == ALLEGRO_EVENT_TIMER) {
            g.moveForwardA();
            g.moveForwardB();
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    g.moveUpB();
                    break;
                case ALLEGRO_KEY_DOWN:
                    g.moveDownB();
                    break;
                case ALLEGRO_KEY_LEFT:
                    g.moveForwardB();
                    break;
                case ALLEGRO_KEY_RIGHT:
                    g.rotateB();
                    break;
                case ALLEGRO_KEY_W:
                    g.moveUpA();
                    break;
                case ALLEGRO_KEY_S:
                    g.moveDownA();
                    break;
                case ALLEGRO_KEY_D:
                    g.moveForwardA();
                    break;
                case ALLEGRO_KEY_A:
                    g.rotateA();
                    break;
                case ALLEGRO_KEY_P:
                // Case used for testing
                    al_stop_timer(timer);
                    break;
                // Case used for testing
                case ALLEGRO_KEY_Q:
                    al_start_timer(timer);
                    break;
                // Case used for testing
                case ALLEGRO_KEY_N:
                    ratio /= 2;
                    al_set_timer_speed(timer, ratio);
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    doExit = 1;
                    break;
            }
        }
    }

    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}
