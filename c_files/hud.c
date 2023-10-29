#include "../headers/hud.h"


HUD hudInit()
{
    HUD hud; 
    
    hud.health = 1000;
    

    return hud;
}

void hudRender(HUD *hud, Player *player, char state)
{
    if(state == 'g')
    {
        DrawRectangle(100, 50, hud->health/2, 50, GOLD);
    }
    if(state == 't')
    {
        DrawText("TITLE les goo", 600, 460, 100, GRAY);
    }
    
}

