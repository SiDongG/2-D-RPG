#include "graphics.h"

#include "globals.h"
#define D 0x3c6500
#define P 0x264000
#define G 0x307400
#define Y 0x7c8b00
#define C 0xce5505
#define M 0xdb8a54
#define S 0xb64901
#define K 0xF5B536

void draw_player(int u, int v)
{
    int player1[11 * 11] =
    {
        Y, S, S, S, S, Y, S, S, S, S, Y,
        S, Y, S, S, S, Y, S, S, S, Y, S,
        S, S, Y, S, Y, S, Y, S, Y, S, S,
        S, S, S, Y, S, S, S, Y, S, S, S,
        S, S, Y, S, S, M, S, S, Y, S, S,
        Y, Y, S, S, M, M, M, S, S, Y, Y,
        S, S, Y, S, S, M, S, S, Y, S, S,
        S, S, S, Y, S, S, S, Y, S, S, S,
        S, S, Y, S, Y, S, Y, S, Y, S, S,
        S, Y, S, S, S, Y, S, S, S, Y, S,
        Y, S, S, S, S, Y, S, S, S, S, Y
    };
    uLCD.BLIT(u, v, 11, 11, player1);
}

void draw_player2(int u, int v)
{
    int player2[11 * 11] =
    {
        K, K, K, K, K, K, K, K, K, K, K,
        K, K, K, K, D, D, K, K, K, K, K,
        K, K, K, P, D, D, D, K, K, K, K,
        K, K, K, K, Y, Y, Y, K, K, K, K,
        K, K, K, K, Y, Y, Y, K, K, K, K,
        K, K, D, D, D, P, D, D, K, K, K,
        K, D, D, K, D, P, D, D, D, D, K,
        D, D, K, K, D, P, D, K, P, K, K,
        K, K, K, K, D, K, D, K, K, K, K,
        K, K, K, K, C, K, C, K, K, K, K,
        K, K, K, C, P, K, P, C, K, K, K
    };
    uLCD.BLIT(u, v, 11, 11, player2);
}

void draw_nothing(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+11, v+11, BLACK);
}

void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = 0xFFFF00; // Yellow
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = 0xD2691E; // "Dirt"
        else if (img[i] == '5') colors[i] = LGREY; // 50% grey
        else if (img[i] == '3') colors[i] = DGREY;
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_plant(int u, int v)
{
    int plantBlock[11 * 11] =
    {
        P, P, P, P, P, P, P, P, P, P, P,
        P, P, P, P, P, P, P, P, P, P, P,
        P, P, D, D, D, D, D, D, D, P, P,
        P, P, D, D, D, D, D, D, D, P, P,
        P, P, D, D, P, P, P, D, D, P, P,
        P, P, D, D, P, D, P, D, D, P, P,
        P, P, D, D, P, P, P, D, D, P, P,
        P, P, D, D, D, D, D, D, D, P, P,
        P, P, D, D, D, D, D, D, D, P, P,
        P, P, P, P, P, P, P, P, P, P, P,
        P, P, P, P, P, P, P, P, P, P, P
    };
    uLCD.BLIT(u, v, 11, 11, plantBlock);
}


void draw_wall(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, WHITE);
}


void draw_player1sprite(int u, int v)
{
//    pc.printf("Drawing Player 1 Sprite\r\n");
    uLCD.filled_rectangle(u, v, u+10, v+10, RED);
}
void draw_player2sprite(int u, int v)
{
//    pc.printf("Drawing Player 2 Sprite\r\n");
    uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
}
void draw_selection( int u, int v){
uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
}

void draw_upper_status()
{
    // Draw bottom border of status bar
    uLCD.line(0, 9, 127, 9, GREEN);
    
    // Add other status info drawing code here
}
 
void draw_lower_status()
{
    // Draw top border of status bar
    uLCD.line(0, 118, 127, 118, GREEN);
    
    // Add other status info drawing code here
}
 
void draw_border()
{
    uLCD.filled_rectangle(0,     9, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE); // Right
}
void draw_treasure(int u, int v) {
    int treasure[11*11] =
    {
        0x307400, 0x307400, 0x307400, 0x307400, 0x307400, 0x307400, 0x307400, 0x307400, 0x307400, 0x307400, 0x307400,
        0x307400, 0x7c8b00, 0x307400, 0x307400, 0x307400, 0x307400, 0x307400, 0x307400, 0x7c8b00, 0x307400, 0x307400,
        0x307400, 0x307400, 0x7c8b00, 0x307400, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x307400, 0x7c8b00, 0x307400,
        0x307400, 0x307400, 0x307400, 0x3e1900, 0x5a2400, 0x5a2400, 0x5a2400, 0x5a2400, 0x3e1900, 0x307400, 0x307400,
        0x307400, 0x307400, 0x307400, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x307400, 0x307400,
        0x307400, 0x307400, 0x3e1900, 0x5a2400, 0x5a2400, 0x5a2400, 0x5a2400, 0x5a2400, 0x5a2400, 0x3e1900, 0x307400,
        0x307400, 0x307400, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x307400,
        0x307400, 0x307400, 0x3e1900, 0x5a2400, 0x5a2400, 0x3e1900, 0x3e1900, 0x5a2400, 0x5a2400, 0x3e1900, 0x307400,
        0x307400, 0x307400, 0x3e1900, 0x5a2400, 0x5a2400, 0x5a2400, 0x5a2400, 0x5a2400, 0x5a2400, 0x3e1900, 0x307400,
        0x307400, 0x307400, 0x3e1900, 0x5a2400, 0x5a2400, 0x5a2400, 0x5a2400, 0x5a2400, 0x5a2400, 0x3e1900, 0x307400,
        0x307400, 0x307400, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x3e1900, 0x307400
    };
    uLCD.BLIT(u, v, 11, 11, treasure);
}

 
 
