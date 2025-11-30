#ifndef SINGLEPLAYER_H
#define SINGLEPLAYER_H
#include"gameCommon.h"
#include <SFML/Graphics.hpp>

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME



// Grid dimensions
const int M = 25;
const int N = 40;

// Global grid
extern int grid[M][N];

// Tile size
extern int ts;

// Enemy object
struct Enemy
{
    int x, y;
    int dx, dy;

    Enemy();
    void move();
};

// Flood fill function for capturing area
void drop(int y, int x);

// Game mode
void singlePlayer();

#endif