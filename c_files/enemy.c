
#include "raylib.h"
#include "../headers/enemy.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../headers/playerHandler.h"

Enemy_array create_enemy_array(Texture2D texture)
{
    Enemy_array Enemy_array;
    Enemy_array.n = 16;
    Enemy_array.used = 0;
    Enemy_array.enemies = malloc(Enemy_array.n * sizeof(Enemy));
    Enemy_array.enemies_p = &Enemy_array.enemies;
    Enemy_array.texture = texture;
    return Enemy_array;
}

Enemy create_enemy(int xpos, int ypos, int width, int height, short id)
{
    Enemy Enemy;
    Enemy.pos.x = xpos;
    Enemy.pos.y = ypos;
   
    Enemy.id = id;
    return Enemy;
}

void enemies_collision(Player *player, Enemy_array *Enemy_array)
{
    for(int i=0; i < Enemy_array->used; i++)
    {
        if(CheckCollisionRecs(Enemy_array->enemies[i].hitbox, player->hitbox))
        {
            Rectangle col = GetCollisionRec(Enemy_array->enemies[i].hitbox, player->hitbox);
           
            if(col.height == col.width)
            {
                if(Enemy_array->enemies[i].hitbox.y == col.y)
                {
                    player->pos.y = player->pos.y - col.height;
                }else{
                    player->pos.x = player->pos.x + col.height;
                }
                if(Enemy_array->enemies[i].hitbox.x == col.x)
                {
                    player->pos.x = player->pos.x - col.width;
                }else{
                    player->pos.x = player->pos.x + col.width;
                }
            }
            else if(col.width > col.height)
            {
                if(Enemy_array->enemies[i].hitbox.y == col.y)
                {
                    player->pos.y = player->pos.y - col.height;
                }else{
                    player->pos.y = player->pos.y + col.height;
                }
            }else {
                if(Enemy_array->enemies[i].hitbox.x == col.x)
                {
                    player->pos.x = player->pos.x - col.width;
                }else{
                    player->pos.x = player->pos.x + col.width;
                }
            }
            player->hitbox.x = player->pos.x;
            player->hitbox.y = player->pos.y;
            
            
        }
    
    }
}

float get_distance(Vector2 pos1, Vector2 pos2) 
{ 
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.x;
    return sqrt(dx*dx + dy*dy); 
}

void enemies_render(Enemy_array *Enemy_array)
{
    Vector2 center = {0, 0};
    for(int i = 0; i < Enemy_array->used; i++)
    {   

        DrawTexturePro(Enemy_array->texture, Enemy_array->enemies[i].srcRect, Enemy_array->enemies[i].destRect, center, 0.0, WHITE);
        DrawRectangleLinesEx(Enemy_array->enemies[i].hitbox, 1.0, RED);
    
    }

}

void follow(Vector2 pos, Vector2 dest, Vector2 *vel)
{
    float dx = (pos.x - dest.x)/fabs(pos.x - dest.x);
    float dy = (pos.y - dest.y)/fabs(pos.y - dest.y);
    vel->x = dx;
    vel->y =dy;
}

void enemies_tick(Enemy_array *Enemy_array, Player *player)
{
    for(int i = 0; i < Enemy_array->used; i++)
    {   

        float angle = get_angle(Enemy_array->enemies[i].pos, player->pos);
        float dist_to_player = get_distance(player->pos, Enemy_array->enemies[i].pos);

        Enemy_array->enemies[i].vel.x = -cos(angle);
        Enemy_array->enemies[i].vel.y = -sin(angle);


        if(dist_to_player > 2)
        {
            Enemy_array->enemies[i].pos.x += Enemy_array->enemies[i].speed * GetFrameTime() * Enemy_array->enemies[i].vel.x;
            Enemy_array->enemies[i].pos.y += Enemy_array->enemies[i].speed * GetFrameTime() * Enemy_array->enemies[i].vel.y;
        }
        
        Enemy_array->enemies[i].destRect.x = Enemy_array->enemies[i].pos.x;
        Enemy_array->enemies[i].destRect.y = Enemy_array->enemies[i].pos.y;

        Enemy_array->enemies[i].hitbox.x = Enemy_array->enemies[i].pos.x;
        Enemy_array->enemies[i].hitbox.y = Enemy_array->enemies[i].pos.y;
    }
}

void init_enemy(Enemy *Enemy, int xpos, int ypos, short id)
{
    int size = 64;
    Enemy->speed = 20;
    Enemy->pos.x = xpos * size;
    Enemy->pos.y = ypos * size;
    
    Enemy->id = id;
   

    Enemy->srcRect.width = size;
    Enemy->srcRect.height = size;
    
    Enemy->srcRect.x = size;
    Enemy->srcRect.y = 0;
    
    //HTIBOX
    Enemy->hitbox.x = xpos * size;
    Enemy->hitbox.y = ypos * size;
    Enemy->hitbox.width = size;
    Enemy->hitbox.height = size;

    Enemy->destRect.x = xpos * size;
    Enemy->destRect.y = ypos * size;
    
    Enemy->destRect.height = size;
    Enemy->destRect.width = size;
}

void add_enemy(Enemy_array *Enemy_array, int xpos, int ypos, short id)
{
    if(Enemy_array->used == Enemy_array->n)
    {
        
        Enemy_array->n = Enemy_array->n * 2;
        Enemy *temp_enemies = NULL;

        temp_enemies = malloc(Enemy_array->n * sizeof(Enemy));


        copy_enemies(Enemy_array->enemies, temp_enemies, Enemy_array->n/2);

        free(Enemy_array->enemies);

        Enemy_array->enemies_p = &temp_enemies;
        Enemy_array->enemies = temp_enemies;
        temp_enemies = NULL;
    }
    Enemy_array->used += 1;

    init_enemy(&Enemy_array->enemies[Enemy_array->used-1], xpos, ypos, id);
    

    
    

}

void delete_enemy(Enemy_array *Enemy_array, int pos)
{

    if(pos != Enemy_array->used -1)
    {
        Enemy_array->enemies[pos].id = Enemy_array->enemies[Enemy_array->used-1].id;

        Enemy_array->enemies[pos].srcRect = Enemy_array->enemies[Enemy_array->used-1].srcRect;
        Enemy_array->enemies[pos].destRect = Enemy_array->enemies[Enemy_array->used-1].destRect;
        Enemy_array->enemies[pos].hitbox = Enemy_array->enemies[Enemy_array->used-1].hitbox;
    }
    Enemy_array->used -= 1;
}

void delete_enemies(Enemy_array *Enemy_array, short id)
{
    int nb = Enemy_array->used;
    for (int i = 0; i < nb; i++)
    {
        if(Enemy_array->enemies[i].id == id)
        {
            delete_enemy(Enemy_array, i);
        }
    }
}

void copy_enemies(Enemy *entry, Enemy *output, int n)
{
    for(int i = 0; i < n; i++)
    {

        output[i].id = entry[i].id;

        output[i].destRect = entry[i].destRect;
        output[i].srcRect = entry[i].srcRect;
        output[i].hitbox = entry[i].hitbox;
    }

}