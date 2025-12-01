#include <SFML/Graphics.hpp>
#include "multiPlayer.h"
#include "profile.h"
#include "authentication.h"
#include <iostream>
#include <time.h>
#include <string>
#include <cstring>
#include <string.h>
#include<iomanip>
#include<iostream>
#include<cmath>

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME

using namespace std;
using namespace sf;

// 1 marks borders
// 0 marks empty space
//-1 marks areas reachable by enemies
// 2 marks player 2's trail
// 3 marks player 1's trail
// 4 marks player 2's captured tiles
// 5 marks player 1's captured tiles

// enemies move in PIXEL coordinates
// player moves in TILE coordinates
// tile size (ts) is 18x18
//  x/ts or y/ts is used to convert PIXEL position to GRID position


GPlayer::GPlayer(int x, int y) {
    this->x = x;
    this->y = y;
    dx = 0;
    dy = 0;
    alive = true;
    points = 0;
    powerups = 0;
    multiplier = 2;
    threshold = 10;
    rewardCounter = 0;
}

void GPlayer::reset() {
    points = 0;
    alive = true;
    multiplier =2;
    threshold = 10; 
    rewardCounter = 0;
}

void GPlayer::die() {
    alive = false;
}

bool GPlayer::isMoving() {
    if(dx !=0 || dy != 0) return true;
    else return false;
}
void GPlayer::clamp() {
    if (x < 0)
        x = 0;
    if (x > N - 1)
        x = N - 1;
    if (y < 0)
        y = 0;
    if (y > M - 1)
        y = M - 1;
}

// When you complete your trail, the game calls drop() starting at each enemy's position. Any empty area reachable
// from an enemy gets marked -1. Everything NOT marked -1 becomes captured territory!

void updatePlayerMovement(GPlayer &player, int ownTrail, int oppTrail){
    player.x += player.dx;
    player.y += player.dy;

    player.clamp();

    int currentCell = grid[player.y][player.x];

    // Check if player hit their own trail or opponent's trail
    if (currentCell == ownTrail || currentCell == oppTrail) {
        player.die();
        return;
    }

    // Only place trail on empty spaces (0)
    // Don't overwrite opponent's captured tiles (4 or 5) or borders (1)
    if (currentCell == 0)
        grid[player.y][player.x] = ownTrail;
}

void handleCapture(GPlayer &player, Enemy a[], int enemyCount, int ownCaptured) {
        // if player reached solid territory - successfully captured
        player.dx = player.dy = 0; // stop moving

        int oldTileCount = 0;
        int newTileCount = 0;

        // Determine own trail value based on captured value
        // If ownCaptured is 5 (player 1), ownTrail is 3
        // If ownCaptured is 4 (player 2), ownTrail is 2
        int ownTrail = ((ownCaptured == 5) ? 3 : 2);

        for (int i = 0; i < M; i++)  {
            for (int j = 0; j < N; j++)
                if (grid[i][j] == ownCaptured)
                    oldTileCount++;
        }

        for (int i = 0; i < enemyCount; i++)
            drop(a[i].y / ts, a[i].x / ts); // for each enemy, call drop() at their positions, this marks all areas reachable by enemy as -1

        for (int i = 0; i < M; i++) // captring logic
            for (int j = 0; j < N; j++)
                if (grid[i][j] == -1) // if the place is reachable by enemy, dont fill it
                    grid[i][j] = 0;
                else if (grid[i][j] == 0 || grid[i][j] == ownTrail) // Only convert own trail, not opponent's
                    grid[i][j] = ownCaptured;

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++)
                if (grid[i][j] == ownCaptured)
                    newTileCount++;
        }

        int newCapturedTiles = newTileCount - oldTileCount;

        int newPts;
        newPts = newCapturedTiles;

        if (newPts > player.threshold) {
            newPts *= player.multiplier;
            player.rewardCounter++;

            if (player.rewardCounter == 3){
                player.threshold = 5;
            }

            if (player.rewardCounter == 5)
                player.multiplier = 4;
        }

        int prevPoints = player.points;
        player.points += newPts;

        if ((prevPoints < 50 && player.points >= 50)) {
            player.powerups++;
        }

        if ((prevPoints < 70 && player.points >= 70))  {
            player.powerups++;
        }

        if (player.points >= 100) {
            int currentMilestones = 1 + (player.points - 100) / 30;
            int previousMilestones = (prevPoints >= 100) ? (1 + (prevPoints - 100) / 30) : 0;

            int newMilestones = currentMilestones - previousMilestones;
            player.powerups += newMilestones;
        }

        // cout << "Capture called for player at (" << player.x << "," << player.y << ") with ownCaptured=" << ownCaptured << " and ownTrail=" << ownTrail << endl;
        // cout << "Old tiles: " << oldTileCount << ", New tiles: " << newTileCount << ", Points awarded: " << newPts << endl;
}

bool isGridFull() {
    for(int i=1; i<M-1; i++) {
        for(int j=1; j<N-1; j++)
            if(grid[i][j] == 0)
                return false;
    }
    return true;
}

void clearGrid() {
    for(int i=1; i<M-1; i++) {
        for(int j=1; j<N-1; j++)
            grid[i][j] = 0;
}
}

void multiPlayer(){ // main game
    srand(time(0));

    Font font;
    font.loadFromFile("Guntech.otf");

    Clock countdownClock;
    Time totalCountdownTime = seconds(60.0f);

    Text deadtext;
    deadtext.setFont(font);
    deadtext.setCharacterSize(25);
    deadtext.setFillColor(Color::Red);
    deadtext.setPosition(200.f, 450.f);

    // draw grid
    RenderWindow window(VideoMode(N * ts, M * ts + 100), "Xonix Game!");
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(true);

    Texture t1, t2, t3; // loading images
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100); // game over is displayed at the center of the screen
    sEnemy.setOrigin(20, 20);        // center of rotation of enemy

    GPlayer player1(0, 10), player2(N - 1, 10);
    int enemyCount = 4;
    Enemy a[10]; // there can be upto 10 enemies, we are using 4

    bool freeze[2];
    freeze[0] = freeze[1] = false;
    float freezeTime[2];
    float freezeDuration = 3;

    bool Game = true;
 
    float timer = 0, delay = 0.07;
    Clock clock;

    // borders
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
                grid[i][j] = 1;

    while (window.isOpen())   {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e)) { // checks if any events happened (mouse click, key press, window close, etc, if any event to process, returns true 
            
            if (e.type == Event::Closed) // player clicked X
                window.close();

            if (e.type == Event::KeyPressed)  {                                       // any key was pressed
                if (e.key.code == Keyboard::Escape) {// check if its the escape key, restart game    {
                    for (int i = 1; i < M - 1; i++)
                        for (int j = 1; j < N - 1; j++)
                            grid[i][j] = 0; // reset everything except borders
                   
                    player1.reset();
                    player1.x = 0;
                    player1.y = 10;
                    player2.reset();
                    player2.x = N-1;
                    player2.y = 10;
                    player1.points = 0;
                    player2.points = 0;

                    countdownClock.restart();

                    clearGrid();
                    Game = true; // restaet game
                }

                if ((e.key.code == Keyboard::P) && player1.powerups > 0) {
                    freeze[0] = true;
                    freezeTime[0] = 0;
                    player1.powerups--;
                }

                if ((e.key.code == Keyboard::W) && player2.powerups > 0) {
                    freeze[1] = true;
                    freezeTime[1] = 0;
                    player2.powerups--;
                }
            }
        }

        // iskeypressed: key is continuously held down
        // keypressed: key is just pressed once
        if (Keyboard::isKeyPressed(Keyboard::Left)) {// move left
            player1.dx = -1;
            player1.dy = 0;
        };
        if (Keyboard::isKeyPressed(Keyboard::Right)){ // move right
            player1.dx = 1;
            player1.dy = 0;
        };
        if (Keyboard::isKeyPressed(Keyboard::Up)){ // move up
            player1.dx = 0;
            player1.dy = -1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) { // move dowjn
            player1.dx = 0;
            player1.dy = 1;
        };


        if (Keyboard::isKeyPressed(Keyboard::A)) {// move left
            player2.dx = -1;
            player2.dy = 0;
        };
        if (Keyboard::isKeyPressed(Keyboard::G)){ // move right
            player2.dx = 1;
            player2.dy = 0;
        };
        if (Keyboard::isKeyPressed(Keyboard::Q)){ // move up
            player2.dx = 0;
            player2.dy = -1;
        }
        if (Keyboard::isKeyPressed(Keyboard::X)) { // move dowjn
            player2.dx = 0;
            player2.dy = 1;
        };

        for(int i=0; i<=1; i++) {
            if (freeze[i]) {
                freezeTime[i] += time;
                if (freezeTime[i] >= freezeDuration){
                    freeze[i] = false;
                    freezeTime[i] = 0;
                }
            }
        }


        if (!Game)
            continue;

        if (timer > delay){
            if(player1.alive && !freeze[1]) {
                bool hadTrail = (player1.dx != 0 || player1.dy != 0);
                updatePlayerMovement(player1, 3, 2);

                // Handle capture immediately after movement while not frozen
                if(hadTrail && ((grid[player1.y][player1.x] == 1) || (grid[player1.y][player1.x] == 5)))
                    handleCapture(player1, a, enemyCount, 5);
            }

            if(!player1.alive) {
                deadtext.setString("PLAYER 1 DEAD!!!!");
            }

            if(player2.alive && !freeze[0]) {
                bool hadTrail = (player2.dx != 0 || player2.dy != 0);
                updatePlayerMovement(player2, 2, 3);

                // Handle capture immediately after movement while not frozen
                if(hadTrail && ((grid[player2.y][player2.x] == 1) || (grid[player2.y][player2.x] == 4)))
                    handleCapture(player2, a, enemyCount, 4);
            }

            if(!player2.alive) {
                deadtext.setString("PLAYER 2 DEAD!!!!");
            }

            if (!player1.alive && !player2.alive) {
                deadtext.setString("BOTH PLAYERS DEAD!!!!");
                Game = false;
            }

            if(player1.x == player2.x && player1.y == player2.y) {
                if(player1.isMoving() && player2.isMoving()) {
                    player1.die();
                    player2.die();

                    deadtext.setString("BOTH PLAYERS DEAD!!!!");
                }

                if(player1.isMoving() && !player2.isMoving()) {
                    player1.die();
                    deadtext.setString("PLAYER 1 DEAD!!!!");
                }

                if(!player1.isMoving() && player2.isMoving()) {
                    player2.die();
                    deadtext.setString("PLAYER 2 DEAD!!!!");
                }
            }

            if (!player1.alive && !player2.alive){
                Game = false;
            }

            timer = 0;
        }

        Time elapsed = countdownClock.getElapsedTime();
        Time remainingTime = totalCountdownTime - elapsed;

        
        if (remainingTime.asSeconds() <= 0.0f)
        { Game = false;}

        if (!freeze[0] && !freeze[1])
            for (int i = 0; i < enemyCount; i++) // move enemies stores in array a
                a[i].move();
        //-----------------------------------------CAPTURING + POINTS --------------------------------------------------------------------------


        for (int i = 0; i < enemyCount; i++)
            if (grid[a[i].y / ts][a[i].x / ts] == 2)   {                 // if the enemy hits ur trail
                player2.die();
                deadtext.setString("PLAYER 2 DEAD!!!!");
            }

         for (int i = 0; i < enemyCount; i++)
            if (grid[a[i].y / ts][a[i].x / ts] == 3)   {                 // if the enemy hits ur trail
                player1.die();
                deadtext.setString("PLAYER 1 DEAD!!!!");
            }

        if(isGridFull()) {
            Game = false; 
        }

        int secondsLeft = (int)round(remainingTime.asSeconds());

        Text timeText;
        timeText.setFont(font);
        timeText.setCharacterSize(25);
        timeText.setFillColor(Color::White);
        timeText.setPosition(300.f, 500.f);
        timeText.setString(to_string(secondsLeft));

        Text scoreText1;
        scoreText1.setFont(font);
        scoreText1.setString("Player 1 \nScore: " + to_string(player1.points) + "\nPowerups: " + to_string(player1.powerups));
        scoreText1.setCharacterSize(15);
        scoreText1.setFillColor(Color::White);
        scoreText1.setPosition(10.f, M * ts + 10.f);

        Text scoreText2;
        scoreText2.setFont(font);
        scoreText2.setString("Player 2 \nScore: " + to_string(player2.points) + "\nPowerups: " + to_string(player2.powerups));
        scoreText2.setCharacterSize(15);
        scoreText2.setFillColor(Color::White);
        scoreText2.setPosition(550.f, M * ts + 10.f);
        /////////draw//////////
        window.clear();

        // drawing tiles

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++){
                if (grid[i][j] == 0 || grid[i][j] == -1) // dont draw anything on enemy space
                    continue;
                if (grid[i][j] == 1)
                    sTile.setTextureRect(IntRect(0, 0, ts, ts)); // filled space (blue ile)
                if (grid[i][j] == 2)
                    sTile.setTextureRect(IntRect(54, 0, ts, ts)); // trail p2
                if (grid[i][j] == 3)
                    sTile.setTextureRect(IntRect(108, 0, ts, ts)); // trail p1
                if (grid[i][j] == 4)
                    sTile.setTextureRect(IntRect(90, 0, ts, ts)); // captured p2
                if (grid[i][j] == 5)
                    sTile.setTextureRect(IntRect(72, 0, ts, ts)); // captured p1
                // IntRect(0, 0, ts, ts) means: start at pixel (0,0) in the image, grab an 18×18 area

                // actually drawing
                sTile.setPosition(j * ts, i * ts); // convert grid to pixel
                window.draw(sTile);
            }

        // drawing player
        sTile.setTextureRect(IntRect(36, 0, ts, ts)); // red rect
        sTile.setPosition(player1.x * ts, player1.y * ts);            // convert grid to pizel
        window.draw(sTile);                           // DRAWWW

        sTile.setTextureRect(IntRect(18, 0, ts, ts));   
        sTile.setPosition(player2.x * ts, player2.y * ts); // convert grid to pizel
        window.draw(sTile);

        sEnemy.rotate(10); // rotate by 10 degrees
        // This happens every frame, making enemies spin continuously

        // draw enemy
        for (int i = 0; i < enemyCount; i++)    {
            sEnemy.setPosition(a[i].x, a[i].y); // set position , directly in pixels since enemies move in pixel
            window.draw(sEnemy);
        }

        if(!player1.alive || !player2.alive)
        window.draw(deadtext);

        if (!Game){
            window.draw(sGameover); // game overrrr


            Text winnerText;
            winnerText.setFont(font);
            winnerText.setCharacterSize(25);
            winnerText.setFillColor(Color::Red);
            winnerText.setPosition(200.f, 470.f);

            if(player1.points > player2.points) {
                winnerText.setString("PLAYER 1 WINS!!!!");
            } else if(player2.points >player1.points) {
                winnerText.setString("PLAYER 2 WINS!!!");
            } else {
                winnerText.setString("TIE!!!!");
            }

            window.draw(winnerText);
        }
        window.draw(scoreText1);

        window.draw(scoreText2);
        window.draw(timeText);

        window.display();
    }

    return;
}

