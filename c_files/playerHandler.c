#include "../headers/playerHandler.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../headers/wall.h"
#include "../headers/hud.h"

void tick(Player *player, WArray *wallArray, Vector2 screen, HUD *Hud)
{
    //PLAYER

    //SPRITE ORIENTATION
    if(player->velX == 0 && player-> velY < 0) player->orient = 0;
    if(player->velX > 0 && player-> velY < 0) player->orient = 1;
    if(player->velX > 0 && player-> velY == 0) player->orient = 2;
    if(player->velX > 0 && player-> velY > 0) player->orient = 3;
    if(player->velX == 0 && player-> velY > 0) player->orient = 4;
    if(player->velX < 0 && player-> velY > 0) player->orient = 5;
    if(player->velX < 0 && player-> velY == 0) player->orient = 6;
    if(player->velX < 0 && player-> velY < 0) player->orient = 7;
    

    //POSITION
    player->pos.x += player->velX*player->speed*GetFrameTime();
    player->pos.y += player->velY*player->speed*GetFrameTime();
    player->angle = player->orient*45 + 90;
   

    //-------rects--------//
    player->hitbox.x = player->pos.x;
    player->hitbox.y = player->pos.y;
    player->pos_c.x = player->pos.x +32;
    player->pos_c.y = player->pos.y +32;
    //WEAPONS

   

    //attacking
    int speed = 25;
    if(player->attacking)
    {
        player->attackCounter ++;
        player->speed += 1; 
        

        if(player->attackCounter >= 25) player->speed = speed;
        if(player->attackCounter >= 32)
        {
            player->attackCounter = 0;
            player->attacking = false;
        }
    }else
    {
        player->speed = speed;

        player->weapon_s.destRect.x = player->screen_c.x + 100*cos(DEG2RAD*(player->angle+35));
        player->weapon_s.destRect.y = player->screen_c.y + 100*sin(DEG2RAD*(player->angle+35));
    }
}

void render(Player *player)
{
    Vector2 a ={0.0, 0.0};
    //ANIMATION ICI
    sprite_set_position(&player->character, player->orient);

    char s[100];
    sprintf(s, "pos : %d, %d", (int)player->pos.x/64, (int)player->pos.y/64);

    DrawFPS(1000, 50);
    DrawText(s, 100, 100, 10, RED);
    
    //rectangle d'attaque
    Rectangle rect = create_rectangle(player->screen_c.x, player->screen_c.y, 250, 100);
    float angle = get_mouse_angle();
    Vector2 rect_c = {-10, rect.height/2};
    
    
    if(IsMouseButtonDown(1) && !player->attacking)
    {
        DrawRectanglePro(rect, rect_c, RAD2DEG*angle, RED);
    }
    

    //weapons
    if(player->attacking)
    {
        DrawTexturePro(player->weapon_s.texture, player->weapon_s.srcRect, player->weapon_s.destRect, player->center_r, (player->angle+105-180), WHITE);
    }else
    {
        DrawTexturePro(player->weapon_s.texture, player->weapon_s.srcRect, player->weapon_s.destRect, player->center_r, (player->angle+105), WHITE);
    }
    
    DrawTexturePro(player->character.texture, player->character.srcRect, player->character.destRect, a, 0.0, WHITE);

}

void keyHandler(Player *player, HUD *hud, char *state)
{
    
    if(*state == 'g')
    {
        if(!player->attacking)
        {
            //-----DEPLACEMENTS-----//
            player->depl[0] = IsKeyDown(KEY_W);
            player->depl[1] = IsKeyDown(KEY_S);
            player->depl[2] = IsKeyDown(KEY_A);
            player->depl[3] = IsKeyDown(KEY_D);
            
            /*
            if(player->depl[0]) player->character.srcRect.y = 120;
            if(player->depl[1]) player->character.srcRect.y = 40;
            if(player->depl[2]) player->character.srcRect.y = 80;
            if(player->depl[3]) player->character.srcRect.y = 0;
            */

            if(player->depl[0] && player->depl[1]) player->velY = 0;
            else if(player->depl[0]) player -> velY = -player->speed;
            else if(player->depl[1]) player -> velY = player->speed;
            else player -> velY = 0;

            if(player->depl[2] && player->depl[3]) player->velX = 0;
            else if(player->depl[2]) player -> velX = -player->speed;
            else if(player->depl[3]) player -> velX = player->speed;
            else player -> velX = 0;

            
            
        }
        
        if(fabs(player->velX) + fabs(player->velY) == 2*player->speed)
        {
            player->velX = player->velX*0.707;
            player->velY = player->velY*0.707;
        }
        
        if(IsMouseButtonReleased(1))
        {
            playerAttack(player); 
        }
        
        
        if(IsKeyPressed(KEY_J))
        {
            playerAttack(player);
        }
        /*
        if(IsMouseButtonPressed(0))
        {
            hud->health -= 10;
        }
        */

    }
    else if(*state == 't')
    {
        if(IsMouseButtonPressed(0))
        {
            *state = 'g';
        }

    }

}

int getOrient(int angle)
{
    int orient;
    if(angle <= 22.5) orient = 2;
    else if(angle <= 22.5+45) orient = 1;
    else if(angle <= 22.5+90) orient = 0;
    else if(angle <= 22.5+135) orient = 7;
    else if(angle <= 22.5+180) orient = 6;
    else if(angle <= 22.5+45+180) orient = 5;
    else if(angle <= 22.5+90+180) orient = 4;
    else if(angle <= 22.5+135+180) orient = 3;
    return orient;
}

void playerAttack(Player *player)
{
    if(!player->attacking)
    {
        player->attacking = true;
        float angle = get_mouse_angle();
        player->velX = cos(angle)*player->speed;
        player->velY = sin(angle)*player->speed;
    }
}

float get_angle(Vector2 pos, Vector2 target)
{
    float dx = target.x - pos.x;
    float dy = target.y - pos.y;
    //FAIRE CAS DX/DY = 0
    return atan2(target.y - pos.y, target.x - pos.x)+PI;
}

float get_mouse_angle()
{
    Vector2 a = {0,0};
    Vector2 mouse = { 992 - GetMouseX(), 572 - GetMouseY()};
    return get_angle(a, mouse);
}

Rectangle create_rectangle(float x, float y, float width, float height)
{  
    Rectangle rect;
    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;
    return rect;
}
