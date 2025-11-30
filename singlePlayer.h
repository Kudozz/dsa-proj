#ifndef SINGLEPLAYER_H
#define SINGLEPLAYER_H
#include"gameCommon.h"
#include <SFML/Graphics.hpp>

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME



// Grid dimensions

// Global grid
extern int grid[M][N];

// Tile size
extern int ts;

// Enemy object

// Flood fill function for capturing area
void drop(int y, int x);

// Game mode
void singlePlayer();

#endif