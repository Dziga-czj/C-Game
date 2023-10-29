#ifndef DUNGEON_H_
#define DUNGEON_H_

#define SIZE 22

#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct WArray WArray;
/* Salles :
Coffre
Shop
enemis
salle base

*/

typedef struct
{
    char type; // 'v' = vide, 'e' = ennemis, 'd' = debut
    bool ouv_g;
    bool ouv_d;
    bool ouv_h;
    bool ouv_b;



} Room;

//-----rooms infinies avec diff expo => force a ameliorer le stuff et grind ??

typedef struct
{   
    Texture *textures;
    Room *rooms;
    int nb_rooms;
    char ***room_mesh;
    
    Room map[7][7];
    int size;

} Dungeon;

Dungeon create_dungeon();

void generate_dungeon(WArray *warray, Dungeon *dungeon);

void delete_dungeon(Dungeon *dungeon, WArray *warray);

void wall_p_back(WArray *warray);

void get_rooms(Dungeon *dungeon);

void print_dungeon(Dungeon *dungeon);

void create_map(Dungeon *dungeon, int x, int y, char dir, int n);



#endif