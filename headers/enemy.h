#ifndef ENEMY_H_
#define ENEMY_H_

#include "playerHandler.h"
#include "raylib.h"

struct Enemy
{
    int hp;
    int atk;
    
    short type;
    float speed;
    short id;
    Vector2 pos;
    Vector2 vel;

    Texture2D sprite;
    
    Rectangle srcRect;
    Rectangle destRect;
    Rectangle hitbox;
};
typedef struct Enemy Enemy;

struct Enemy_array
{
    int used;
    int n;

    Texture2D texture;

    Enemy **enemies_p;
    Enemy *enemies;
};
typedef struct Enemy_array Enemy_array;

Enemy_array create_enemy_array(Texture2D texture);

void enemies_collision(Player *player, Enemy_array *Enemy_array);

void enemies_render(Enemy_array *Enemy_array);

void enemies_tick(Enemy_array *Enemy_array, Player *player);

void init_enemy(Enemy *Enemy, int xpos, int ypos, short id);

Enemy create_enemy(int xpos, int ypos, int width, int height, short id);

void add_enemy(Enemy_array *Enemy_array, int xpos, int ypos, short id);

void delete_enemy(Enemy_array *Enemy_array, int pos);

void delete_enemies(Enemy_array *Enemy_array, short id);

void copy_enemies(Enemy *entry, Enemy *output, int n);



#endif