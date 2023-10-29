#ifndef HANDLER_H_
#define HANDLER_H_

typedef struct HUD HUD;
typedef struct GameObject GameObject;
typedef struct Player Player;
typedef struct WArray WArray;

#include "raylib.h"
#include "sprite.h"

struct Player
{

    //position
    float velX;
    float velY;
    float angle;
    float speed;
    short orient;
    bool depl[4];

    //centre ecran
    Vector2 screen_c; //milieu ecran = ~ 1920/2, 1080/2
    Vector2 center_r; // centre relatif = 32, 32
    
    Vector2 pos; // vraie position du joueur
    Vector2 pos_c; // vraie position du joueur centrée

    //textures personnage
    Sprite character;
    //weapons
    short curentWeapon;
    Sprite weapon_s;

    //player rects
    Rectangle hitbox;

    //status
    bool attacking;
    int attackCounter;

};

typedef struct Player Player;

//---------------------GAME----------------------//
void playerAttack(Player *player);

void keyHandler(Player *player, HUD *hud, char *state);

void render(Player *player);

void tick(Player *player, WArray *wallArray, Vector2 screen, HUD *hud);

float get_angle(Vector2 pos, Vector2 target);

float get_mouse_angle();

Rectangle create_rectangle(float x, float y, float width, float height);

#endif