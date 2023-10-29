#ifndef SPRITE_H_
#define SPRITE_H_

#include "raylib.h"

struct Sprite
{
    Rectangle srcRect;
    Rectangle destRect;
    Texture2D texture;
    int position;
    int size;

}; 
typedef struct Sprite Sprite;

Sprite sprite_init(int x, int y, int width, int height, int size, Texture2D texture);

void sprite_set_position(Sprite *sprite, int position);


#endif