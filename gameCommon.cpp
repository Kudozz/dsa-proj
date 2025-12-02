#include"gameCommon.h"
#include<time.h>
#include<cstdlib>

// whole grid
int grid[M][N] = {0};
int ts = 18; // tile size

void drop(int y, int x) // flood fill captured tiles
{
    if (grid[y][x] == 0) // current cell empty , mark as reachable by enemy
        grid[y][x] = -1;
    if (grid[y - 1][x] == 0) // check cell ABOVE current one (y-1) and recursivecall
        drop(y - 1, x);
    if (grid[y + 1][x] == 0) // check cell BELOW (y+1)
        drop(y + 1, x);
    if (grid[y][x - 1] == 0) // check cell on LEFT (x-1)
        drop(y, x - 1);
    if (grid[y][x + 1] == 0) // check cell on RIGHT (x+1)
        drop(y, x + 1);
}

Enemy::Enemy() {
    x = y = 300;//starting position, center of the grid
    dx = 4 - rand() % 8;//random speeds
    dy = 4 - rand() % 8;
}

void Enemy::move() {
    x += dx;//move in x direction
    int cellValue = grid[y / ts][x / ts];
    if (cellValue == 1 || cellValue == 4 || cellValue == 5)//if a solid tile or captured tile is hit, reverse direction (bounce)
    {
        dx = -dx; // reverse speed
        x += dx; // keep moving
    }

    y += dy;//move in y direction
    cellValue = grid[y / ts][x / ts];
    if (cellValue == 1 || cellValue == 4 || cellValue == 5)//solid line or captured tile hit, reverse direction
    {
        dy = -dy;//reverse dir
        y += dy; // keep moving
    }
}

void clearGrid() {
    for(int i=1; i<M-1; i++) {
        for(int j=1; j<N-1; j++)
            grid[i][j] = 0;
}
}