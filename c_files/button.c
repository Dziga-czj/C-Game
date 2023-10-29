#include "../headers/button.h"
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

Button button_create(int x, int y, int width, int height, char state)
{
    Button button;
    button.destRect.x = x;
    button.destRect.y = y;
    button.destRect.width = width;
    button.destRect.height = height;
    button.state = state;
    button.srcRect.x = 0;
    button.srcRect.y = 0;
    button.srcRect.width = 250;
    button.srcRect.height = 100;
    button.pressed = false;
    return button;
}

ButtonArray bArray_create(int n, Texture2D texture)
{
    ButtonArray bArray;
    bArray.buttons = (Button *) malloc(n*sizeof(Button));
    bArray.n = n;
    bArray.used = 0;
    bArray.texture = texture;
    return bArray;
}

void button_set_pressed(Button *button, bool state)
{
    button->pressed = state;
    if(state)
    {
        button->srcRect.x = 250;
    }else
    {
        button->srcRect.x = 0;
    }
}

void add_button(ButtonArray *bArray, int x, int y, int width, int height, char state)
{
    bArray->used += 1;
    if(bArray->used <= bArray->n)
    {
        bArray->buttons[bArray->used-1] = button_create(x, y, width, height, state);
    }else 
    {
        printf("probleme de taille du tableau boutons\n");
    }
}

void button_tick(ButtonArray *bArray, Vector2 mouse, char *state)
{
    for (int i = 0; i < bArray->used; i++)
    {
        if(bArray->buttons[i].state == *state)
        {
            if(CheckCollisionPointRec(mouse, bArray->buttons[i].destRect))
            {
                button_set_pressed(&bArray->buttons[i], true);
                if(IsMouseButtonPressed(0))
                {
                    *state = 'g';
                }
            }else {
                button_set_pressed(&bArray->buttons[i], false);
            }
            
        }

    }
}

void button_render(ButtonArray *bArray, char state)
{
    Vector2 a = {0,0};
    for (int i = 0; i < bArray->used; i++)
    {   
        if(bArray->buttons[i].state == state)
        {
            DrawTexturePro(bArray->texture, bArray->buttons[i].srcRect, bArray->buttons[i].destRect, a, 0, WHITE);
        }

    }
}