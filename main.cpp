// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

#include <math.h>
#include<stdio.h>

#define D 0x3c6500
#define P 0x264000
#define G 0x307400
#define Y 0x7c8b00
#define C 0xce5505
#define M 0xdb8a54
#define S 0xb64901

// Helper function declarations
int get_action (GameInputs in);
int update_game (int action);
void draw_game (int init);
void playSound(char* wav);
int go_right(int x, int y);
int go_left(int x, int y);
int go_up(int x, int y);
int go_down(int x, int y);
int Mode=0;
int Player=1;
int Char=1;
int Char2=1;
int Num_player1=3;
int Num_player2=3;


// Top down camera view
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
} Camera;

struct{
    int x,y;    // Current locations
    int px, py; // Previous locations
    int HP;
    int A;
    int D1;
    int M1;
}player1;

struct{
    int x,y;    // Current locations
    int px, py; // Previous locations
    int HP;
    int A;
    int D1;
    int M1;
}player2;

struct{
    int x,y;    // Current locations
    int px, py; // Previous locations
    int HP;
    int A;
    int D1;
    int M1;
}player12;

struct{
    int x,y;    // Current locations
    int px, py; // Previous locations
    int HP;
    int A;
    int D1;
    int M1;
}player13;

struct{
    int x,y;    // Current locations
    int px, py; // Previous locations
    int HP;
    int A;
    int D1;
    int M1;
}player22;

struct{
    int x,y;    // Current locations
    int px, py; // Previous locations
    int HP;
    int A;
    int D1;
    int M1;
}player23;
/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define BACK_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define INFO_BUTTON 7

int get_action(GameInputs inputs)
{
    if (!inputs.b1) return ACTION_BUTTON;
    if (!inputs.b2) return BACK_BUTTON;
    if (!inputs.b3) return INFO_BUTTON;
    if (inputs.ay >= 0.50) return GO_UP;
    if (inputs.ay < -0.50) return GO_DOWN;
    if (inputs.ax < -0.50) return GO_LEFT;
    if (inputs.ax >= 0.50) return GO_RIGHT;

    else return NO_ACTION;
}


/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 4
#define FULL_DRAW 2

#define MODE_FREE_ROAM 1
#define MODE_SELECTED 2
int update_game(int action)
{
    // Save player previous location before updating
    Camera.px = Camera.x;
    Camera.py = Camera.y;
    player1.px = player1.x;
    player1.py = player1.y; 
    player2.px = player2.x;
    player2.py = player2.y;
    player12.px = player12.x;
    player12.py = player12.y; 
    player13.px = player13.x;
    player13.py = player13.y;
    player22.px = player22.x;
    player22.py = player22.y; 
    player23.px = player23.x;
    player23.py = player23.y;
    
    // Do different things based on the action
    // You can define smaller functions that get called for each case
    if (Mode==0){
        switch(action) {
            case GO_UP:
                Camera.y--; 
                return FULL_DRAW;
            case GO_LEFT:
                Camera.x--; return FULL_DRAW;
            case GO_DOWN:
                Camera.y++; return FULL_DRAW;
            case GO_RIGHT:
                Camera.x++; return FULL_DRAW;
            case ACTION_BUTTON:
                if (get_here(Camera.x,Camera.y)->type==PLAYER1 || get_here(Camera.x,Camera.y)->type==PLAYER2){
                   if (get_here(Camera.x,Camera.y)->type==PLAYER1){
                       Player=1;
                       if (get_here(Camera.x,Camera.y)->A==player1.A){
                           Char=1;
                           }
                       if (get_here(Camera.x,Camera.y)->A==player12.A){
                           Char=2;
                           }
                       if (get_here(Camera.x,Camera.y)->A==player13.A){
                           Char=3;
                           }
                       }
                   if (get_here(Camera.x,Camera.y)->type==PLAYER2){
                       Player=2;
                       if (get_here(Camera.x,Camera.y)->A==player2.A){
                           Char2=1;
                           }
                       if (get_here(Camera.x,Camera.y)->A==player22.A){
                           Char2=2;
                           }
                       if (get_here(Camera.x,Camera.y)->A==player23.A){
                           Char2=3;
                           }
                       }
                   Mode=1;
                   if (Player==1){
                   draw_game(5);
                   }
                   if (Player==2){
                   draw_game(6);
                   }
                   }
            case BACK_BUTTON:
                break;
            case INFO_BUTTON:
                draw_game(4);
                return GAME_OVER;
            default:
                break;    
        }
        return NO_RESULT;
    }
    if (Mode==1){
        switch(action) {
            case GO_UP:
                if (go_up(Camera.px,Camera.py)) {
                    return FULL_DRAW;
                    }
                else break;
            case GO_LEFT:
                if (go_left(Camera.px,Camera.py)) {
                    return FULL_DRAW;
                    }
                else break;
            case GO_DOWN:
                if (go_down(Camera.px,Camera.py)) {
                    return FULL_DRAW;
                    }                
                else break;
            case GO_RIGHT:
                if (go_right(Camera.px,Camera.py)) {
                    return FULL_DRAW;
                    }
                else break;
            case ACTION_BUTTON:
                if (Player==1 && Char==1){
                if ((player1.x!=Camera.x || player1.y!=Camera.y)){
                    if (player1.x-Camera.x<player1.M1 && -player1.M1<player1.x-Camera.x && player1.y-Camera.y<player1.M1 && -player1.M1<player1.y-Camera.y){
                     player1.x=Camera.x;
                     player1.y=Camera.y;
                     add_player(player1.x, player1.y, player1.HP, player1.A, player1.D1, player1.M1);
                     map_erase(player1.px, player1.py);
                     player1.px=player1.x;
                     player1.py=player1.y;
                     Mode=0;
                    }
                    if (get_west(player1.x,player1.y)->type==PLAYER2){
                    draw_game(9);
                    player1.HP=player1.HP-get_west(player1.x,player1.y)->A+player1.D1;
                    get_here(player1.x,player1.y)->HP=player1.HP;
                    get_west(player1.x,player1.y)->HP=get_west(player1.x,player1.y)->HP-player1.A+get_west(player1.x,player1.y)->D1;
                    if (get_west(player1.x,player1.y)->HP<0){
                          map_erase(player1.x-1,player1.y);
                          Num_player2-=1;
                          }
                    }
                    if (get_east(player1.x,player1.y)->type==PLAYER2){
                    draw_game(9);
                    player1.HP=player1.HP-get_east(player1.x,player1.y)->A+player1.D1;
                    get_here(player1.x,player1.y)->HP=player1.HP;
                    get_east(player1.x,player1.y)->HP=get_east(player1.x,player1.y)->HP-player1.A+get_east(player1.x,player1.y)->D1;
                        if (get_east(player1.x,player1.y)->HP<0){
                          map_erase(player1.x+1,player1.y);
                          Num_player2-=1;
                          }
                    }
                    if (get_north(player1.x,player1.y)->type==PLAYER2){
                    draw_game(9);
                    player1.HP=player1.HP-get_north(player1.x,player1.y)->A+player1.D1;
                    get_here(player1.x,player1.y)->HP=player1.HP;
                    get_north(player1.x,player1.y)->HP=get_north(player1.x,player1.y)->HP-player1.A+get_north(player1.x,player1.y)->D1;
                         if (get_north(player1.x,player1.y)->HP<0){
                          map_erase(player1.x,player1.y-1);
                          Num_player2-=1;
                          }
                    }
                    if (get_south(player1.x,player1.y)->type==PLAYER2){
                    draw_game(9);
                    player1.HP=player1.HP-get_south(player1.x,player1.y)->A+player1.D1;
                    get_here(player1.x,player1.y)->HP=player1.HP;
                    get_south(player1.x,player1.y)->HP=get_south(player1.x,player1.y)->HP-player1.A+get_south(player1.x,player1.y)->D1;
                        if (get_south(player1.x,player1.y)->HP<0){
                          map_erase(player1.x,player1.y+1);
                          Num_player2-=1;
                          }
                    }
                    return FULL_DRAW;
                    }
                }
                if (Player==1 && Char==2){
                if ((player12.x!=Camera.x || player12.y!=Camera.y)){
                    if (player12.x-Camera.x<player12.M1 && -player12.M1<player12.x-Camera.x && player12.y-Camera.y<player12.M1 && -player12.M1<player12.y-Camera.y){
                     player12.x=Camera.x;
                     player12.y=Camera.y;
                     add_player(player12.x, player12.y, player12.HP, player12.A, player12.D1, player12.M1);
                     map_erase(player12.px, player12.py);
                     player12.px=player12.x;
                     player12.py=player12.y;
                     Mode=0;
                     }
                    if (get_west(player12.x,player12.y)->type==PLAYER2){
                    draw_game(9);
                    player12.HP=player12.HP-get_west(player12.x,player12.y)->A+player12.D1;
                    get_here(player12.x,player12.y)->HP=player12.HP;
                    get_west(player12.x,player12.y)->HP=get_west(player12.x,player12.y)->HP-player12.A+get_west(player12.x,player12.y)->D1;
                       if (get_west(player12.x,player12.y)->HP<0){
                          map_erase(player12.x-1,player12.y);
                          Num_player2-=1;
                          }
                    }
                    if (get_east(player12.x,player12.y)->type==PLAYER2){
                    draw_game(9);
                    player12.HP=player12.HP-get_east(player12.x,player12.y)->A+player12.D1;
                    get_here(player12.x,player12.y)->HP=player12.HP;
                    get_east(player12.x,player12.y)->HP=get_east(player12.x,player12.y)->HP-player12.A+get_east(player12.x,player12.y)->D1;
                        if (get_east(player12.x,player12.y)->HP<0){
                          map_erase(player12.x+1,player12.y);
                          Num_player2-=1;
                          }
                    }
                    if (get_north(player12.x,player12.y)->type==PLAYER2){
                    draw_game(9);
                    player12.HP=player12.HP-get_north(player12.x,player12.y)->A+player12.D1;
                    get_here(player12.x,player12.y)->HP=player1.HP;
                    get_north(player12.x,player12.y)->HP=get_north(player12.x,player12.y)->HP-player12.A+get_north(player12.x,player12.y)->D1;
                        if (get_north(player12.x,player12.y)->HP<0){
                          map_erase(player12.x,player12.y-1);
                          Num_player2-=1;
                          }
                    }
                    if (get_south(player12.x,player12.y)->type==PLAYER2){
                    draw_game(9);
                    player12.HP=player12.HP-get_south(player12.x,player12.y)->A+player12.D1;
                    get_here(player12.x,player12.y)->HP=player12.HP;
                    get_south(player12.x,player12.y)->HP=get_south(player12.x,player12.y)->HP-player12.A+get_south(player12.x,player12.y)->D1;
                        if (get_south(player12.x,player12.y)->HP<0){
                          map_erase(player12.x,player12.y+1);
                          Num_player2-=1;
                          }
                    }
                    return FULL_DRAW;
                    }
                }
                if (Player==1 && Char==3){
                if ((player13.x!=Camera.x || player13.y!=Camera.y)){
                    if (player13.x-Camera.x<player13.M1 && -player13.M1<player13.x-Camera.x && player13.y-Camera.y<player13.M1 && -player13.M1<player13.y-Camera.y){
                      player13.x=Camera.x;
                      player13.y=Camera.y;
                      add_player(player13.x, player13.y, player13.HP, player13.A, player13.D1, player13.M1);
                      map_erase(player13.px, player13.py);
                      player13.px=player13.x;
                      player13.py=player13.y;
                      Mode=0;
                      }
                    if (get_west(player13.x,player13.y)->type==PLAYER2){
                    draw_game(9);
                    player13.HP=player13.HP-get_west(player13.x,player13.y)->A+player13.D1;
                    get_here(player13.x,player13.y)->HP=player13.HP;
                    get_west(player13.x,player13.y)->HP=get_west(player13.x,player13.y)->HP-player13.A+get_west(player13.x,player13.y)->D1;
                        if (get_west(player13.x,player13.y)->HP<0){
                          map_erase(player13.x-1,player13.y);
                          Num_player2-=1;
                          }
                    }
                    if (get_east(player13.x,player13.y)->type==PLAYER2){
                    draw_game(9);
                    player13.HP=player13.HP-get_east(player13.x,player13.y)->A+player13.D1;
                    get_here(player13.x,player13.y)->HP=player13.HP;
                    get_east(player13.x,player13.y)->HP=get_east(player13.x,player13.y)->HP-player13.A+get_east(player13.x,player13.y)->D1;
                        if (get_east(player13.x,player13.y)->HP<0){
                          map_erase(player13.x+1,player13.y);
                          Num_player2-=1;
                          }
                    }
                    if (get_north(player13.x,player13.y)->type==PLAYER2){
                    draw_game(9);
                    player13.HP=player13.HP-get_north(player13.x,player13.y)->A+player13.D1;
                    get_here(player13.x,player13.y)->HP=player13.HP;
                    get_north(player13.x,player13.y)->HP=get_north(player13.x,player13.y)->HP-player13.A+get_north(player13.x,player13.y)->D1;
                        if (get_north(player13.x,player13.y)->HP<0){
                          map_erase(player13.x,player13.y-1);
                          Num_player2-=1;
                          }
                    }
                    if (get_south(player13.x,player13.y)->type==PLAYER2){
                    draw_game(9);
                    player13.HP=player13.HP-get_south(player13.x,player13.y)->A+player13.D1;
                    get_here(player13.x,player13.y)->HP=player13.HP;
                    get_south(player13.x,player13.y)->HP=get_south(player13.x,player13.y)->HP-player13.A+get_south(player13.x,player13.y)->D1;
                        if (get_south(player13.x,player13.y)->HP<0){
                          map_erase(player13.x,player13.y+1);
                          Num_player2-=1;
                          }
                    }
                    return FULL_DRAW;
                    }
                }
                if (Player==2 && Char2==1){
                if ((player2.x!=Camera.x || player2.y!=Camera.y)){
                   if (player2.x-Camera.x<player2.M1 && -player2.M1<player2.x-Camera.x && player2.y-Camera.y<player2.M1 && -player2.M1<player2.y-Camera.y){
                     player2.x=Camera.x;
                     player2.y=Camera.y;
                     add_player2(player2.x, player2.y ,player2.HP, player2.A, player2.D1, player2.M1);
                     map_erase(player2.px, player2.py);
                     player2.px=player2.x;
                     player2.py=player2.y;
                     Mode=0;
                     }
                   if (get_west(player2.x,player2.y)->type==PLAYER1){
                    draw_game(9);
                    player2.HP=player2.HP-get_west(player2.x,player2.y)->A+player2.D1;
                    get_here(player2.x,player2.y)->HP=player2.HP;
                    get_west(player2.x,player2.y)->HP=get_west(player2.x,player2.y)->HP-player2.A+get_west(player2.x,player2.y)->D1;
                      if (get_west(player2.x,player2.y)->HP<0){
                          map_erase(player2.x-1,player2.y);
                          Num_player1-=1;
                          }
                    }
                    if (get_east(player2.x,player2.y)->type==PLAYER1){
                    draw_game(9);
                    player2.HP=player2.HP-get_east(player2.x,player2.y)->A+player2.D1;
                    get_here(player2.x,player2.y)->HP=player2.HP;
                    get_east(player2.x,player2.y)->HP=get_east(player2.x,player2.y)->HP-player2.A+get_east(player2.x,player2.y)->D1;
                        if (get_east(player2.x,player2.y)->HP<0){
                          map_erase(player2.x+1,player2.y);
                          Num_player1-=1;
                          }
                    }
                    if (get_north(player2.x,player2.y)->type==PLAYER1){
                    draw_game(9);
                    player2.HP=player2.HP-get_north(player2.x,player2.y)->A+player2.D1;
                    get_here(player2.x,player2.y)->HP=player2.HP;
                    get_north(player2.x,player2.y)->HP=get_north(player2.x,player2.y)->HP-player2.A+get_north(player2.x,player2.y)->D1;
                         if (get_north(player2.x,player2.y)->HP<0){
                          map_erase(player2.x,player2.y-1);
                          Num_player1-=1;
                          }
                    }
                    if (get_south(player2.x,player2.y)->type==PLAYER1){
                    draw_game(9);
                    player2.HP=player2.HP-get_south(player2.x,player2.y)->A+player2.D1;
                    get_here(player2.x,player2.y)->HP=player2.HP;
                    get_south(player2.x,player2.y)->HP=get_south(player2.x,player2.y)->HP-player2.A+get_south(player2.x,player2.y)->D1;
                        if (get_south(player2.x,player2.y)->HP<0){
                          map_erase(player2.x,player2.y+1);
                          Num_player1-=1;
                          }
                    }
                    return FULL_DRAW;
                    }
                }
                if (Player==2 && Char2==2){
                if ((player22.x!=Camera.x || player22.y!=Camera.y)){
                    if (player22.x-Camera.x<player22.M1 && -player22.M1<player22.x-Camera.x && player22.y-Camera.y<player22.M1 && -player22.M1<player22.y-Camera.y){
                     player22.x=Camera.x;
                     player22.y=Camera.y;
                     add_player2(player22.x, player22.y ,player22.HP, player22.A, player22.D1, player22.M1);
                     map_erase(player22.px, player22.py);
                     player22.px=player22.x;
                     player22.py=player22.y;
                     Mode=0;
                     }
                    if (get_west(player22.x,player22.y)->type==PLAYER1){
                    draw_game(9);
                    player22.HP=player22.HP-get_west(player22.x,player22.y)->A+player22.D1;
                    get_here(player22.x,player22.y)->HP=player22.HP;
                    get_west(player22.x,player22.y)->HP=get_west(player22.x,player22.y)->HP-player22.A+get_west(player22.x,player22.y)->D1;
                      if (get_west(player22.x,player22.y)->HP<0){
                          map_erase(player22.x-1,player22.y);
                          Num_player1-=1;
                          }
                    }
                    if (get_east(player22.x,player22.y)->type==PLAYER1){
                    draw_game(9);
                    player2.HP=player22.HP-get_east(player22.x,player22.y)->A+player22.D1;
                    get_here(player22.x,player22.y)->HP=player22.HP;
                    get_east(player22.x,player22.y)->HP=get_east(player22.x,player22.y)->HP-player22.A+get_east(player22.x,player22.y)->D1;
                        if (get_east(player22.x,player22.y)->HP<0){
                          map_erase(player22.x+1,player22.y);
                          Num_player1-=1;
                          }
                    }
                    if (get_north(player22.x,player22.y)->type==PLAYER1){
                    draw_game(9);
                    player22.HP=player22.HP-get_north(player22.x,player22.y)->A+player22.D1;
                    get_here(player22.x,player22.y)->HP=player22.HP;
                    get_north(player22.x,player22.y)->HP=get_north(player22.x,player22.y)->HP-player22.A+get_north(player22.x,player22.y)->D1;
                         if (get_north(player22.x,player22.y)->HP<0){
                          map_erase(player22.x,player22.y-1);
                          Num_player1-=1;
                          }
                    }
                    if (get_south(player22.x,player22.y)->type==PLAYER1){
                    draw_game(9);
                    player22.HP=player22.HP-get_south(player22.x,player22.y)->A+player22.D1;
                    get_here(player22.x,player22.y)->HP=player22.HP;
                    get_south(player22.x,player22.y)->HP=get_south(player22.x,player22.y)->HP-player22.A+get_south(player22.x,player22.y)->D1;
                        if (get_south(player22.x,player22.y)->HP<0){
                          map_erase(player22.x,player22.y+1);
                          Num_player1-=1;
                          }
                    }
                    return FULL_DRAW;
                    }
                }
                if (Player==2 && Char2==3){
                if ((player23.x!=Camera.x || player23.y!=Camera.y)){
                   if (player23.x-Camera.x<player23.M1 && -player23.M1<player23.x-Camera.x && player23.y-Camera.y<player23.M1 && -player23.M1<player23.y-Camera.y){
                     player23.x=Camera.x;
                     player23.y=Camera.y;
                     add_player2(player23.x, player23.y ,player23.HP, player23.A, player23.D1, player23.M1);
                     map_erase(player23.px, player23.py);
                     player23.px=player23.x;
                     player23.py=player23.y;
                     Mode=0;
                     }
                   if (get_west(player23.x,player23.y)->type==PLAYER1){
                    draw_game(9);
                    player23.HP=player23.HP-get_west(player23.x,player23.y)->A+player23.D1;
                    get_here(player23.x,player23.y)->HP=player23.HP;
                    get_west(player23.x,player23.y)->HP=get_west(player23.x,player23.y)->HP-player23.A+get_west(player23.x,player23.y)->D1;
                      if (get_west(player23.x,player23.y)->HP<0){
                          map_erase(player23.x-1,player23.y);
                          Num_player1-=1;
                          }
                    }
                    if (get_east(player23.x,player23.y)->type==PLAYER1){
                    draw_game(9);
                    player23.HP=player23.HP-get_east(player23.x,player23.y)->A+player23.D1;
                    get_here(player23.x,player23.y)->HP=player23.HP;
                    get_east(player23.x,player23.y)->HP=get_east(player23.x,player23.y)->HP-player23.A+get_east(player23.x,player23.y)->D1;
                        if (get_east(player23.x,player23.y)->HP<0){
                          map_erase(player23.x+1,player23.y);
                          Num_player1-=1;
                          }
                    }
                    if (get_north(player23.x,player23.y)->type==PLAYER1){
                    draw_game(9);
                    player23.HP=player23.HP-get_north(player23.x,player23.y)->A+player23.D1;
                    get_here(player23.x,player23.y)->HP=player23.HP;
                    get_north(player23.x,player23.y)->HP=get_north(player23.x,player23.y)->HP-player23.A+get_north(player23.x,player23.y)->D1;
                         if (get_north(player23.x,player23.y)->HP<0){
                          map_erase(player23.x,player23.y-1);
                          Num_player1-=1;
                          }
                    }
                    if (get_south(player23.x,player23.y)->type==PLAYER1){
                    draw_game(9);
                    player23.HP=player23.HP-get_south(player23.x,player23.y)->A+player23.D1;
                    get_here(player23.x,player23.y)->HP=player23.HP;
                    get_south(player23.x,player23.y)->HP=get_south(player23.x,player23.y)->HP-player23.A+get_south(player23.x,player23.y)->D1;
                        if (get_south(player23.x,player23.y)->HP<0){
                          map_erase(player23.x,player23.y+1);
                          Num_player1-=1;
                          }
                    }
                    return FULL_DRAW;
                    }
                }
            case BACK_BUTTON:
                Mode=0;
                if (Player==1){
                draw_game(6);
                }
                if (Player==2){
                draw_game(5);
                }
                break;
            case INFO_BUTTON:
                draw_game(7);
                break;
            default:
                break;    
        }
        return NO_RESULT;
        }
        return NO_RESULT;
}

int go_right(int x, int y)
{
    MapItem *item = get_east(x+0.5, y); //get item to right
    MapItem *item2 = get_east(x+0.5,y-1);
    if ( (item->walkable && item2->walkable)) { // check if walkable
        Camera.x++;
        return 1;
    } else return 0;
}

int go_left(int x, int y)
{
    MapItem *item = get_west(x-1, y); //get item to left
    MapItem *item2 = get_west(x-1,y-1);
    if ( (item->walkable && item2->walkable))  { // check if walkable
        Camera.x--;
        return 1;
    } else return 0;
}

int go_up(int x, int y)
{
    MapItem *item = get_north(x-1, y-1); //get item to north
    MapItem *item2 = get_north(x, y-1);
    MapItem *item3 = get_north(x-1, y);
    if ((item->walkable && item2->walkable && item3->walkable)) { // check if walkable
        Camera.y--;
        return 1;
    } else return 0;
}

int go_down(int x, int y)
{
    MapItem *item = get_south(x, y+0.5); //get item to south
    MapItem *item2 = get_south(x-1, y+0.5);
    if ((item->walkable && item2->walkable)) { // check if walkable
        Camera.y++;
        return 1;
    } else return 0;
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */

void draw_game(int init)
{
    if (Num_player1==0 or Num_player2==0){
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        uLCD.locate(2,4);
        uLCD.textbackground_color(BLACK);
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.color(WHITE);
        uLCD.printf("Game");
        uLCD.locate(2,6);
        uLCD.printf("Over");
        wait(100);
        }
    if (init == 4) {
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        uLCD.locate(2,4);
        uLCD.textbackground_color(BLACK);
        uLCD.text_width(1.5);
        uLCD.text_height(1.5);
        uLCD.color(WHITE);
        uLCD.printf("Game Over");
        wait(1);
    }
    if (init == 5) {
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        uLCD.locate(2,2);
        uLCD.textbackground_color(BLACK);
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.color(WHITE);
        uLCD.printf("Player1 Turn");
        wait(1);
    }
    if (init == 6) {
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        uLCD.locate(2,2);
        uLCD.textbackground_color(BLACK);
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.color(WHITE);
        uLCD.printf("Player2 Turn");
        wait(1);
    }
    if (init == 7) {
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        uLCD.textbackground_color(BLACK);
        uLCD.text_width(1);
        uLCD.text_height(1);
        uLCD.color(WHITE);
        uLCD.locate(1,2);
        uLCD.printf(" HP: %d",get_here(Camera.x,Camera.y)->HP);
        uLCD.locate(1,4);
        uLCD.printf(" Attack: %d",get_here(Camera.x,Camera.y)->A);
        uLCD.locate(1,6);
        uLCD.printf(" Defense: %d",get_here(Camera.x,Camera.y)->D1);
        uLCD.locate(1,8);
        uLCD.printf(" Movement: %d",get_here(Camera.x,Camera.y)->M1);
        wait(3);
        }
    if (init == 9) {
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        uLCD.textbackground_color(BLACK);
        uLCD.text_width(1);
        uLCD.text_height(1);
        uLCD.color(WHITE);
        uLCD.locate(1,2);
        uLCD.printf("ATTTTTACK");
        wait(1);
        }
    // Draw game border first
    if (init==15){
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        uLCD.textbackground_color(BLACK);
        uLCD.text_width(1);
        uLCD.text_height(1);
        uLCD.color(WHITE);
        uLCD.locate(1,2);
        uLCD.printf("2-D RPG Game");
        uLCD.locate(1,4);
        uLCD.printf("you have 3 characters, each with different status, if you defeat all enemies you win!!");
        uLCD.locate(1,10);
        uLCD.printf("p1:attack p2:back p3:info");
        wait(5);    
        }
    if(init) draw_border();

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
        for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Camera.x;
            int y = j + Camera.y;

            // Compute the previous map (px, py) of this tile
            int px = i + Camera.px;
            int py = j + Camera.py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (i == 0 && j == 0) {
                if (Mode==1 && Player==1){
                    draw=draw_player;
                    }
                else if (Mode==1 && Player==2){
                    draw=draw_player2;
                    }// Decide what to draw at the 0 position
                else {draw = draw_selection;}
            } 
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) { // Current (i,j) in the map
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) { // Only draw if they're different
                    if (curr_item) { // There's something here! Draw it
                        draw = curr_item->draw;
                    } else { // There used to be something, but now there isn't
                        draw = draw_nothing;
                    }
                }
            } else if (init) { // If doing a full draw, but we're out of bounds, draw the walls.
                draw = draw_wall;
            }
            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars
    draw_upper_status();
    draw_lower_status();
}

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    // "Random" plants
    Map* map = set_active_map(0);
    add_plant(3, 3);
    
    for(int i = map_width() + 3; i < map_area(); i += 39) {
        add_plant(i % map_width(), i / map_width());
    }
    pc.printf("plants\r\n");
    add_wall(0, 8, HORIZONTAL, 14);
    add_wall(0, 18, HORIZONTAL, 16);
    add_wall(16, 8, VERTICAL, 11);
    add_wall(20, 0, VERTICAL, 18);
    add_wall(24, 0, VERTICAL, 22);
    add_wall(4, 22, HORIZONTAL, 21);
    add_wall(0, 26, HORIZONTAL, 45);
    //Initialize characters//
    player1.x=15;
    player1.y=17;
    player1.HP=101;
    player1.A=180;
    player1.D1=18;
    player1.M1=100;
    add_player(player1.x, player1.y,player1.HP, player1.A,player1.D1,player1.M1);
    player12.x=23;
    player12.y=25;
    player12.HP=102;
    player12.A=25;
    player12.D1=0;
    player12.M1=5;
    add_player(player12.x, player12.y,player12.HP, player12.A,player12.D1,player12.M1);
    player13.x=10;
    player13.y=17;
    player13.HP=103;
    player13.A=50;
    player13.D1=10;
    player13.M1=5;
    add_player(player13.x, player13.y,player13.HP, player13.A,player13.D1,player13.M1);
    player2.x=17;
    player2.y=19;
    player2.HP=150;
    player2.A=25;
    player2.D1=10;
    player2.M1=50;
    add_player2(player2.x, player2.y,player2.HP, player2.A,player2.D1,player2.M1);
    player22.x=17;
    player22.y=24;
    player22.HP=10;
    player22.A=100;
    player22.D1=10;
    player22.M1=10;
    add_player2(player22.x, player22.y,player22.HP, player22.A,player22.D1,player22.M1);
    player23.x=11;
    player23.y=11;
    player23.HP=50;
    player23.A=40;
    player23.D1=10;
    player23.M1=10;
    add_player2(player23.x, player23.y,player23.HP, player23.A,player23.D1,player23.M1);
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");

    print_map();
}


/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");

    pc.printf("Program Starting");
    // Initialize the maps
    maps_init();
    init_main_map();
    
    Camera.x = Camera.px = 13;
    Camera.y = Camera.px = 17;
    
    // Initialize Characters

    // Initialize game state
    set_active_map(0);

    // Initial drawing
    draw_game(15);
    draw_game(true);

    // Main game loop
    while(1) {
        // Timer to measure game update speed
        Timer t;
        t.start();

        GameInputs inputs = read_inputs();// 1. Read inputs
        int action = get_action(inputs);// 2. Determine action (move, act, menu, etc.)
        int init = update_game(action);// 3. Update game
        if (init == 4){
            break;
        }// 3b. Check for game over
        draw_game(init);// 4. Draw screen

        // Compute update time
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
}

