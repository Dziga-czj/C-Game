#include "raylib.h"

struct Item{
    char type;
    int att;
    int armor;
    char enchant;
};
typedef struct Item Item;

struct LootArray{
    int tmax;
    int tact;
    Item items;
};
typedef struct LootArray LootArray;
    
struct Chest{
    char type;
    int x;
    int y;
    LootArray loot;
};
typedef struct Chest Chest;
    



    
