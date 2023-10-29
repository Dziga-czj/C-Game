#ifndef BUTTON_H_
#define BUTTON_H_

#include "raylib.h"

struct Button
{   
    char state;
    Rectangle destRect;
    Rectangle srcRect;
    bool pressed;
};
typedef struct Button Button;

struct ButtonArray
{
    int n;
    int used;
    Button *buttons;
    Texture2D texture;

};
typedef struct ButtonArray ButtonArray;

Button button_create(int x, int y, int width, int height, char state);

void add_button(ButtonArray *bArray, int x, int y, int width, int height, char state);

ButtonArray bArray_create(int n, Texture2D texture);

void button_tick(ButtonArray *bArray, Vector2 mouse, char *state);

void button_set_pressed(Button *button, bool state);

void button_render(ButtonArray *bArray, char state);

#endif