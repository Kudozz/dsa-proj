#ifndef GAMECOMMON_H
#define GAMECOMMON_H

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

#endif