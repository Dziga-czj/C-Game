#include "../headers/wall.h"
#include <stdlib.h>
#include <stdio.h>
#include "../headers/playerHandler.h"

WArray createWallArray(Texture2D texture)
{
    WArray warray;
    warray.n = 2;
    warray.used = 0;
    warray.walls = malloc(warray.n * sizeof(Wall));
    warray.walls_p = &warray.walls;
    warray.texture = texture;
    return warray;
}

Wall createWall(int xpos, int ypos, int width, int height, short id)
{
    Wall wall;
    wall.xpos = xpos;
    wall.ypos = ypos;
    wall.width = width;
    wall.height = height;
    wall.id = id;
    return wall;
}

void wallCollision(Player *player, WArray *warray)
{
    for(int i=0; i < warray->used; i++)
    {
        if(CheckCollisionRecs(warray->walls[i].hitbox, player->hitbox))
        {
            Rectangle col = GetCollisionRec(warray->walls[i].hitbox, player->hitbox);
           
            if(col.height == col.width)
            {
                if(warray->walls[i].hitbox.y == col.y)
                {
                    player->pos.y = player->pos.y - col.height;
                }else{
                    player->pos.x = player->pos.x + col.height;
                }
                if(warray->walls[i].hitbox.x == col.x)
                {
                    player->pos.x = player->pos.x - col.width;
                }else{
                    player->pos.x = player->pos.x + col.width;
                }
            }
            else if(col.width > col.height)
            {
                if(warray->walls[i].hitbox.y == col.y)
                {
                    player->pos.y = player->pos.y - col.height;
                }else{
                    player->pos.y = player->pos.y + col.height;
                }
            }else {
                if(warray->walls[i].hitbox.x == col.x)
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

void wallRender(WArray *warray)
{
    Vector2 center = {0, 0};
    for(int i = 0; i < warray->used; i++)
    {   
        if(warray->walls[i].id != 0)
        {
            
            DrawTexturePro(warray->texture, warray->walls[i].srcRect, warray->walls[i].destRect, center, 0.0, WHITE);
            //DrawRectangleLinesEx(warray->walls[i].hitbox, 1.0, RED);
        }
    }

}

void wallTick(WArray *warray)
{
    /*
    for(int i = 0; i < warray->used; i++)
    {   
        warray->walls[i].destRect.x = warray->walls[i].pos.x;
        warray->walls[i].destRect.y = warray->walls[i].pos.y;
    }
    */

}

void initWall(Wall *wall, int xpos, int ypos, int width, int height, short id, char orient)
{
    wall->xpos = xpos;
    wall->ypos = ypos;
    wall->width = width;
    wall->height = height;
    wall->id = id;
    wall->orient = orient;

    int size = 64;
    //SRCRECT
    
    wall->srcRect.width = size;
    wall->srcRect.height = size;
    switch(orient)
    {
        case 'b':
            wall->srcRect.x = 1 * wall->srcRect.width;
        break;
        case 'h':
            wall->srcRect.x = 0;
        break;
        case 'g':
            wall->srcRect.x = 2 * wall->srcRect.width;
        break;
        case 'd':
            wall->srcRect.x = 3 * wall->srcRect.width;
        break;
        case '1':
            wall->srcRect.x = 1 * wall->srcRect.width;
        break;
        case 'v':
            wall->srcRect.x = 1 * wall->srcRect.width;
        break;
        default:
        break;

    }
    
    //wall->srcRect.x = wall->id * wall->srcRect.width;
    //POS
    //wall->pos.y = wall->ypos * wall->srcRect.height;
    //wall->pos.x = wall->xpos * wall->srcRect.width;
    //DESTRUCTLOL (HITBOX)
    wall->hitbox.x = xpos * size;
    wall->hitbox.y = ypos * size;
    wall->hitbox.width = wall->width * size;
    wall->hitbox.height = wall->height * size;

    wall->destRect.x = xpos * size;
    wall->destRect.y = ypos * size;
    
    //sprite offset
    //wall->destRect.y = wall->pos.y - 20.0;
    //wall->destRect.x = wall->pos.x;

    wall->destRect.height = wall->hitbox.height;
    wall->destRect.width = wall->hitbox.width;
}

void addWall(WArray *warray, int xpos, int ypos, int width, int height, short id, char orient)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if(warray->used == warray->n)
            {
                
                warray->n = warray->n * 2;
                Wall *tempWalls = NULL;

                tempWalls = malloc(warray->n * sizeof(Wall));


                copyWalls(warray->walls, tempWalls, warray->n/2);

                free(warray->walls);

                warray->walls_p = &tempWalls;
                warray->walls = tempWalls;
                tempWalls = NULL;
            }
            warray->used += 1;

            initWall(&warray->walls[warray->used-1], xpos+i, ypos+j, 1, 1, id, orient);
        }
    }
    
    

}

void deleteWall(WArray *warray, int pos)
{

    if(pos != warray->used -1)
    {
        warray->walls[pos].xpos = warray->walls[warray->used-1].xpos;
        warray->walls[pos].ypos = warray->walls[warray->used-1].ypos;

        warray->walls[pos].width = warray->walls[warray->used-1].width;
        warray->walls[pos].height = warray->walls[warray->used-1].height;

        warray->walls[pos].id = warray->walls[warray->used-1].id;

        warray->walls[pos].srcRect = warray->walls[warray->used-1].srcRect;
        warray->walls[pos].destRect = warray->walls[warray->used-1].destRect;
        warray->walls[pos].hitbox = warray->walls[warray->used-1].hitbox;
    }
    warray->used -= 1;
}

void delete_walls(WArray *warray, short id)
{
    int nb = warray->used;
    for (int i = 0; i < nb; i++)
    {
        if(warray->walls[i].id == id)
        {
            deleteWall(warray, i);
        }
    }
}

void copyWalls(Wall *entry, Wall *output, int n)
{
    for(int i = 0; i < n; i++)
    {
        output[i].xpos = entry[i].xpos;
        output[i].ypos = entry[i].ypos;

        output[i].height = entry[i].height;
        output[i].width = entry[i].width;
        //output[i].pos = entry[i].pos;
        
        
        output[i].id = entry[i].id;

        output[i].destRect = entry[i].destRect;
        output[i].srcRect = entry[i].srcRect;
        output[i].hitbox = entry[i].hitbox;
    }

}



