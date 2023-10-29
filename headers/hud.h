
#ifndef HUD_H_
#define HUD_H_

#include "raylib.h"

typedef struct HUD HUD;
typedef struct Player Player;

struct HUD
{
    int health;
    



};

typedef struct HUD HUD;

HUD hudInit();
void hudRender(HUD *hud, Player *player, char state);

#endif