
#include <stdio.h>
#include "raylib.h"
#include <math.h>
#include <stdlib.h>

#include "../headers/dungeon.h"
#include "../headers/sprite.h"
#include "../headers/playerHandler.h"
#include "../headers/hud.h"
#include "../headers/wall.h"
#include "../headers/button.h"
#include "../headers/enemy.h"


/*/----------------------TODO-----------------------/

enemis
attaques

ui


*/

void initPlayer(Player *player, Vector2 screen)
{
    player->attackCounter = 0;
    player->velX=0;
    player->velY=0;
    player->angle=0;
    player->speed=0;
    player->orient=0;
    player->attacking = false;

    player->pos.x = screen.x/2;
    player->pos.y = screen.y/2;

    player->center_r.x = 32;
    player->center_r.y = 32;  

    //player->posEcran = player ->pos;
    
    player->hitbox.height = 64;
    player->hitbox.width = 64;
    
    player->character = sprite_init(player->pos.x, player->pos.y, 64, 64, 64, LoadTexture("ressources/sprites/player.png"));
    //player->character.srcRect.width = 40;
    //player->character.srcRect.height = 40;
    

    player->screen_c.x = player->pos.x + player->hitbox.width/2;
    player->screen_c.y = player->pos.y + player->hitbox.height/2;

    player->orient = 4;

    //player-> center.x = 32;
    //player-> center.y = 32;

    //WEAPONS
    player->curentWeapon = 0;
    player->weapon_s = sprite_init(player->pos.x-32, player->pos.y-32, 20, 120, 0, LoadTexture("ressources/sprites/longsword.png"));
    
    // NO HITBOX
    //player->hitbox.width =0;
    //player->hitbox.height =0;
    
}

void initWalls(WArray *warray)
{   
    //-------------WALL CREATION-------------//
    
    addWall(warray, 5, 5, 4, 2, 2, 'v');
    addWall(warray, 5, 7, 5, 2, 2, 'g');
    


    
    
}

void initButtons(ButtonArray *bArray)
{
    add_button(bArray, 860, 600, 300, 200, 't');


}

int main(void)
{
    Vector2 screen = {1920, 1080};

    InitWindow(screen.x, screen.y, "Game");

    char state = 'g';

    HUD hud = hudInit();

    float mouseX;
    float mouseY;

    Vector2 mouse;

    //Vector2 offset = {0.0, 0.0};

    SetTargetFPS(120);               

    Player player;
 
    WArray wallArray = createWallArray(LoadTexture("ressources/sprites/walls.png"));
    initWalls(&wallArray);

    initPlayer(&player, screen);
    
    ButtonArray bArray = bArray_create(10, LoadTexture("ressources/sprites/boutons.png"));
    initButtons(&bArray);
    
    Texture2D textureHerbe = LoadTexture("ressources/sprites/herbe1.png");

    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.pos.x + 32.0f, player.pos.y + 32.0f };
    camera.offset = (Vector2){ screen.x/2.0f, screen.y/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    player.pos.x = 0;
    player.pos.y = 0;

    Dungeon dungeon = create_dungeon();
    bool in_dungeon = false;

    Enemy_array enemies_array = create_enemy_array(LoadTexture("ressources/sprites/boutons.png"));
    add_enemy(&enemies_array, -1, -1, 0);
    //game loop
    while (!WindowShouldClose())   
    {
        
        if(IsKeyPressed(KEY_K) && !in_dungeon)
        {
            generate_dungeon(&wallArray, &dungeon);   
            print_dungeon(&dungeon);
            in_dungeon = true;
        }
        if(IsKeyPressed(KEY_L))
        {
            delete_dungeon(&dungeon, &wallArray);
            in_dungeon = false;
        }

        mouse.x = GetMouseX();
        mouse.y = GetMouseY();
        //--------------------------------------TICKS-------------------------------------//
        keyHandler(&player, &hud, &state);

        if(state == 'g')
        {
            tick(&player, &wallArray, screen, &hud);
            wallCollision(&player, &wallArray);

            wallTick(&wallArray);
            enemies_tick(&enemies_array, &player);
            //angle = atan((mouseY-y)/(mouseX-x));

            camera.target = (Vector2){ player.pos.x, player.pos.y };       
        }

        button_tick(&bArray, mouse, &state);
       //------------------------------------------RENDER-------------------------------------//
        BeginDrawing();
            ClearBackground(WHITE);

            if(state == 'g')
            {
                BeginMode2D(camera);
                //----------MOBILE----------//

                wallRender(&wallArray);
                enemies_render(&enemies_array);
                EndMode2D();

                //----------FIXE----------//
                render(&player);
            }
            
            button_render(&bArray, state);
            
            
            hudRender(&hud, &player, state);
            
            
            

        EndDrawing();
        
    }

    free(wallArray.walls);
    free(enemies_array.enemies);
    CloseWindow();
    return 0;
}
