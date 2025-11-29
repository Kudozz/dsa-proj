#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int M = 25;
const int N = 40;

//whole grid
int grid[M][N] = {0};
int ts = 18; // tile size

//1 marks borders/captured tiles
//0 marks empty space
//-1 marks areas reachable by enemies
//2 marks your trail

//enemies move in PIXEL coordinates 
//player moves in TILE coordinates 
//tile size (ts) is 18x18 
// x/ts or y/ts is used to convert PIXEL position to GRID position

struct Enemy
{
    int x, y, dx, dy;//x and y is position, dx and dy is speed

    Enemy()
    {
        x = y = 300;//starting position, center of the grid
        dx = 4 - rand() % 8;//random speeds
        dy = 4 - rand() % 8;
    }

    void move()
    {
        x += dx;//move in x direction
        if (grid[y / ts][x / ts] == 1)//if a solid tile is hit, reverse direction (bounce)
        {
            dx = -dx; // reverse speed
            x += dx; // keep moving
        }

        y += dy;//move in y direction
        if (grid[y / ts][x / ts] == 1)//solid line hit, reverse direction
        { 
            dy = -dy;//reverse dir
            y += dy; // keep moving
        }
    }
};

// When you complete your trail, the game calls drop() starting at each enemy's position. Any empty area reachable
// from an enemy gets marked -1. Everything NOT marked -1 becomes captured territory!

void drop(int y, int x) //flood fill captured tiles
{
    if (grid[y][x] == 0) //current cell empty , mark as reachable by enemy
        grid[y][x] = -1;
    if (grid[y - 1][x] == 0) //check cell ABOVE current one (y-1) and recursivecall 
        drop(y - 1, x);
    if (grid[y + 1][x] == 0) // check cell BELOW (y+1) 
        drop(y + 1, x);
    if (grid[y][x - 1] == 0) //check cell on LEFT (x-1)
        drop(y, x - 1);
    if (grid[y][x + 1] == 0) // check cell on RIGHT (x+1)
        drop(y, x + 1);
}

void singlePlayer() {//main game
    srand(time(0));

    //draw grid
    RenderWindow window(VideoMode(N * ts, M * ts), "Xonix Game!");
    window.setFramerateLimit(60);

    Texture t1, t2, t3;//loading images
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100);//game over is displayed at the center of the screen
    sEnemy.setOrigin(20, 20);//center of rotation of enemy

    int enemyCount = 4;
    Enemy a[10];//there can be upto 10 enemies, we are using 4

    bool Game = true;
    int x = 0, y = 0, dx = 0, dy = 0;   
    float timer = 0, delay = 0.07;
    Clock clock;

    //draw borders 
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
                grid[i][j] = 1;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e)) // checks if any events happened (mouse click, key press, window close, etc, if any event to process, returns true
        {
            if (e.type == Event::Closed)//player clicked X
                window.close();

            if (e.type == Event::KeyPressed) //any key was pressed
                if (e.key.code == Keyboard::Escape) // check if its the escape key, restart game
                {
                    for (int i = 1; i < M - 1; i++)
                        for (int j = 1; j < N - 1; j++)
                            grid[i][j] = 0; //reset everything except borders

                    x = 10;
                    y = 0;//player pos is at (10, 0)
                    Game = true; //restaet game
                }
        }

        //iskeypressed: key is continuously held down
        //keypressed: key is just pressed once
        if (Keyboard::isKeyPressed(Keyboard::Left))//move left
        {
            dx = -1;
            dy = 0;
        };
        if (Keyboard::isKeyPressed(Keyboard::Right)) //move right
        {
            dx = 1;
            dy = 0;
        };
        if (Keyboard::isKeyPressed(Keyboard::Up)) // move up
        {
            dx = 0;
            dy = -1;
        };
        if (Keyboard::isKeyPressed(Keyboard::Down)) //move dowjn
        {
            dx = 0;
            dy = 1;
        };

        if (!Game)
            continue;

        if (timer > delay)
        {
            x += dx;
            y += dy;

            //following ifs make sure the player doesnt go offscreen - hitting boundaries means you bounce back
            if (x < 0)
                x = 0;
            if (x > N - 1)
                x = N - 1;
            if (y < 0)
                y = 0;
            if (y > M - 1)
                y = M - 1;

            if (grid[y][x] == 2)//if player hits their own path, game is over
                Game = false;
            if (grid[y][x] == 0) //if moving into empty space mark it as trail
                grid[y][x] = 2;
            timer = 0;
        }

        for (int i = 0; i < enemyCount; i++)//move enemies stores in array a 
            a[i].move();

        if (grid[y][x] == 1) // if player reached solid territory
        { 
            dx = dy = 0; //stop moving

            for (int i = 0; i < enemyCount; i++)
                drop(a[i].y / ts, a[i].x / ts); //for each enemy, call drop() at their positions, this marks all areas reachable by enemy as -1

            for (int i = 0; i < M; i++) // captring logic 
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == -1) // if the place is reachable by enemy, dont fill it
                        grid[i][j] = 0;
                    else
                        grid[i][j] = 1; // mark as solid
        }

        for (int i = 0; i < enemyCount; i++) 
            if (grid[a[i].y / ts][a[i].x / ts] == 2) // if the enemy hits ur trail
                Game = false; //game overrrrrrrrrrr 

        /////////draw//////////
        window.clear();

        //drawing tiles

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (grid[i][j] == 0)//dont draw anything on enemy space
                    continue;
                if (grid[i][j] == 1)
                    sTile.setTextureRect(IntRect(0, 0, ts, ts)); //filled space (blue ile)
                if (grid[i][j] == 2)
                    sTile.setTextureRect(IntRect(54, 0, ts, ts)); //trail (green tile)
                // IntRect(0, 0, ts, ts) means: start at pixel (0,0) in the image, grab an 18×18 area

                //actually drawing
                sTile.setPosition(j * ts, i * ts); //convert grid to pixel
                window.draw(sTile);
            }

        //drawing player
        sTile.setTextureRect(IntRect(36, 0, ts, ts));//red rect
        sTile.setPosition(x * ts, y * ts); // convert grid to pizel
        window.draw(sTile); //DRAWWW

        sEnemy.rotate(10); //rotate by 10 degrees
        // This happens every frame, making enemies spin continuously

        //draw enemy
        for (int i = 0; i < enemyCount; i++)
        {
            sEnemy.setPosition(a[i].x, a[i].y); //set position , directly in pixels since enemies move in pixel
            window.draw(sEnemy);
        }

        if (!Game)
            window.draw(sGameover);//game overrrr

        window.display();
    }

    return;
}