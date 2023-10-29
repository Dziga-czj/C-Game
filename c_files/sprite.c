#include "../headers/sprite.h"

Sprite sprite_init (int x, int y, int width, int height, int size, Texture2D texture)
{
    //initialisation du sprite avec la taille destination
    Sprite sprite;
    sprite.texture = texture;
    sprite.position = 0;
    sprite.destRect.width = width;
    sprite.destRect.height = height;
    sprite.destRect.x = x;
    sprite.destRect.y = y;

    if (size != 0) sprite.srcRect.width = size-2;
    else sprite.srcRect.width = texture.width;
    sprite.srcRect.height = texture.height-2;
    sprite.srcRect.x = 1;
    sprite.srcRect.y = 1;
    return sprite;
}

void sprite_set_position(Sprite *sprite, int position)
{
    sprite->srcRect.x = position*(sprite->srcRect.width+2)+1;
}