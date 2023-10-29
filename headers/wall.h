#ifndef WALL_H_
#define WALL_H_

#include "raylib.h"

typedef struct Player Player;

struct Wall
{
    int xpos;
    int ypos;

    int width;
    int height;

    short id;
    char orient;

    Rectangle destRect;
    Rectangle hitbox;
    Rectangle srcRect;
};
typedef struct Wall Wall;

struct WArray
{
    int used;
    int n;

    Texture2D texture;

    Wall **walls_p;
    Wall *walls;
};
typedef struct WArray WArray;

WArray createWallArray(Texture2D texture);

void wallCollision(Player *player, WArray *warray);

void wallRender(WArray *warray);

void wallTick(WArray *warray);

void initWall(Wall *wall, int xpos, int ypos, int width, int height, short id, char orient);

Wall createWall(int xpos, int ypos, int width, int height, short id);

void addWall(WArray *warray, int xpos, int ypos, int width, int height, short id, char orient);

void deleteWall(WArray *warray, int pos);

void delete_walls(WArray *warray, short id);

void copyWalls(Wall *entry, Wall *output, int n);

#endif