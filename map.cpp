#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

#define NUM_MAPS 2
static Map maps[NUM_MAPS];
static int active_map;
int key;
int x;
int y;
int value;
/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y)
 {
    int x = X * 256;//any power of 2 greater than H
    unsigned ret = x + Y;
    return ret;
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    return key%256;
}

void maps_init()
{
    maps[0].items = createHashTable(map_hash,256);
    maps[0].w=60;
    maps[0].h=60;
    // TODO: Implement!    
    // Initialize hash table
    // Set width & height
}

Map* get_active_map()
{
    return &maps[active_map];
}

Map* set_active_map(int m)
{
    active_map = m;
    return &maps[active_map];
}

void print_map()
{
    char lookup[] = {'W', 'D', 'P', 'A', 'K', 'C', 'N',' ','S'};
    Map* map = get_active_map();
    for(int j = 0; j < map->h; j++)
    {
        for (int i = 0; i < map->w; i++)
        {
            MapItem* item = (MapItem*)getItem(map->items, XY_KEY(i, j));
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    Map *map=get_active_map();
    return map->w;
}

int map_height()
{
    Map *map=get_active_map();
    return map->h;
}

int map_area()
{
    Map *map=get_active_map();
    return map->h*map->w;
}

MapItem* get_current(int x, int y)
{
}

MapItem* get_north(int x, int y)
{
    Map *map=get_active_map();
    int key=XY_KEY(x,y-1);
    return (MapItem*)getItem(map->items,key);
}

MapItem* get_south(int x, int y)
{
    Map *map=get_active_map();
    int key=XY_KEY(x,y+1);
    return (MapItem*)getItem(map->items,key);
}

MapItem* get_east(int x, int y)
{
    Map *map=get_active_map();
    int key=XY_KEY(x+1,y);
    return (MapItem*)getItem(map->items,key);
}

MapItem* get_west(int x, int y)
{
    Map *map=get_active_map();
    int key=XY_KEY(x-1,y);
    return (MapItem*)getItem(map->items,key);
}

MapItem* get_here(int x, int y)
{
    Map *map=get_active_map();
    int key=XY_KEY(x,y);
    return (MapItem*)getItem(map->items,key);
}


void map_erase(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x,y);
    MapItem* Item = (MapItem*) getItem(map->items,key);
    if(Item->type == PLANT || Item ->type == WALL ||Item->type == PLAYER1 ||Item->type == PLAYER2) deleteItem(map->items,key);
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}


void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_player(int x, int y, int HP, int A, int D, int M)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLAYER1;
    w1->draw = draw_player;
    w1->walkable = true;
    w1->HP = HP;
    w1->A  = A;
    w1->D1  = D;
    w1->M1  = M;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_player2(int x, int y, int HP, int A, int D, int M)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLAYER2;
    w1->draw = draw_player2;
    w1->walkable = true;
    w1->HP = HP;
    w1->A  = A;
    w1->D1  = D;
    w1->M1  = M;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}


void add_treasure(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = TREASURE;
    w1->draw = draw_treasure;
    w1->walkable = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

