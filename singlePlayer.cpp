#include <SFML/Graphics.hpp>
#include <time.h>
#include"inventory.h"
#include"profile.h"
#include"authentication.h"
#include<string>
#include<cstring>
#include<string.h>
#include"singlePlayer.h"

using namespace std;
using namespace sf;

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME





// 1 marks borders/captured tiles
// 0 marks empty space
// -1 marks areas reachable by enemies
// 2 marks your trail

// enemies move in PIXEL coordinates 
// player moves in TILE coordinates 
// tile size (ts) is 18x18 
// x/ts or y/ts is used to convert PIXEL position to GRID position



//When you complete your trail, the game calls drop() starting at each enemy's position. Any empty area reachable
// from an enemy gets marked -1. Everything NOT marked -1 becomes captured territory!

void singlePlayer() {//main game
    srand(time(0));

    if(isUserLoggedIn()){ //last theme prefernce
        getInventoryInstance().loadThemePreference(getCurrentUser());
    }

    Font font;
    font.loadFromFile("Guntech.otf"); 

     //draw grid
    RenderWindow window(VideoMode(N * ts, M * 35), "Xonix Game!");
    window.setFramerateLimit(60);

    //theme assets
    ThemeAssets themeAssets= getInventoryInstance().getThemeAssets(getInventoryInstance().getCurrentTheme());
    ThemeColors themeColors= getInventoryInstance().getThemeColors(getInventoryInstance().getCurrentTheme());

    //loading textures
    Texture tTile, tGameover, tEnemy, tBackground;
    Sprite sTile, sGameover, sEnemy, sBackground;
    
    if(!tTile.loadFromFile(themeAssets.tileTexture)){
        tTile.loadFromFile("images/tiles.png");//default

    }

    sTile.setTexture(tTile);
    
    //load enemy
    if(!tEnemy.loadFromFile(themeAssets.enemyTexture)){
        tEnemy.loadFromFile("images/enemy.png");//default enemmy

    }

    sEnemy.setTexture(tEnemy);
    sEnemy.setOrigin(20, 20);
    
    //load background if the theme has one
    if(themeAssets.hasBackground){

        if(tBackground.loadFromFile(themeAssets.backgroundTexture)){

            sBackground.setTexture(tBackground);
            
            sBackground.setScale(  //scaling bg to fit window
                (float)(N * ts)/ tBackground.getSize().x,
                (float)(M * ts)/ tBackground.getSize().y

            );

        }

    }
    
    //game over
    tGameover.loadFromFile("images/gameover.png");
    sGameover.setTexture(tGameover);
    sGameover.setPosition(100, 100);

    //bg color(if there iis no background image)
    Color backgroundColor(themeColors.r_background,themeColors.g_background,themeColors.b_background);

    int enemyCount = 4;
    Enemy a[10];//there can be upto 10 enemies, we are using 4

    bool freeze= false;
    float freezeTime = 0;
    float freezeDuration = 3;

    bool Game = true;
    int x = 0, y = 0, dx = 0, dy = 0;   
    float timer = 0, delay = 0.07;
    Clock clock;

    //draw borders
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
                grid[i][j] = 1;

      int points = 0; 
        int multiplier = 2; 
        int threshold = 10; 
        int rewardCounter = 0, powerups = 0;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

       Event e;
        while (window.pollEvent(e)) // checks if any events happened (mouse click, key press, window close, etc, if any event to process, returns true
        {
            if (e.type == Event::Closed)//player clicked X
                window.close();

            if (e.type == Event::KeyPressed) { //any key was pressed
                if (e.key.code == Keyboard::Escape) // check if its the escape key, restart game
                {
                    for (int i = 1; i < M - 1; i++)
                        for (int j = 1; j < N - 1; j++)
                            grid[i][j] = 0; //reset everything except borders
                    points = 0;
                    threshold=10;
                    multiplier = 2; 
                    rewardCounter = 0;
                    x = 10;
                    y = 0;//player pos is at (10, 0)
                    freeze =false;
                    freezeTime=0;
                    Game = true; //restaet game
                }

                if ((e.key.code == Keyboard::P) && powerups > 0){
                    freeze = true;
                    freezeTime = 0;
                    powerups--;
                }
            }
        }

        //iskeypressed: key is continuously held down
        //keypressed: key is just pressed once

        if (Keyboard::isKeyPressed(Keyboard::Left))//move left
        {
            dx = -1;
            dy = 0;
        };
        if (Keyboard::isKeyPressed(Keyboard::Right))//move right
        {
            dx = 1;
            dy = 0;
        };
        if (Keyboard::isKeyPressed(Keyboard::Up))//move up
        {
            dx = 0;
            dy = -1;
        };
        if (Keyboard::isKeyPressed(Keyboard::Down))//move down
        {
            dx = 0;
            dy = 1;
        };

        if (freeze) {
            freezeTime += time;
            if (freezeTime >= freezeDuration)  {
                freeze = false;
                freezeTime = 0;
            }
        }

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

             if (grid[y][x] == 2) {//if player hits their own path, game is over
                Game = false;
                points= 0;
            }
            if (grid[y][x] == 0) //if moving into empty space mark it as trail
                grid[y][x] = 2;
            timer = 0;
        }

        if(!freeze)
        for (int i = 0; i < enemyCount; i++)//move enemies stores in array a 
            a[i].move();
//-----------------------------------------CAPTURING + POINTS --------------------------------------------------------------------------
      
        if (grid[y][x] == 1) { // if player reached solid territory - successfully captured  
            dx = dy = 0; //stop moving

            int oldTileCount = 0;
            int newTileCount = 0;

            for(int i=0; i<M; i++) {
                for(int j=0; j<N; j++) 
                    if(grid[i][j] == 1)
                        oldTileCount++;
            }

            for(int i=0; i<enemyCount;i++)
                drop(a[i].y / ts, a[i].x / ts);//for each enemy, call drop() at their positions, this marks all areas reachable by enemy as -1

            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == -1)// if the place is reachable by enemy, dont fill it
                        grid[i][j] = 0;
                    else
                        grid[i][j] = 1; // mark as solid
            
            for(int i=0; i<M; i++) {
                for(int j=0; j<N; j++) 
                    if(grid[i][j] == 1)
                        newTileCount++;
            }

            int newCapturedTiles = newTileCount - oldTileCount;

            int newPts;
            newPts = newCapturedTiles; 

            if(newPts > threshold) {
                newPts *= multiplier; 
                rewardCounter++; 

                if(rewardCounter == 3) {
                    threshold = 5; 
                }

                if(rewardCounter == 5)
                    multiplier = 4; 
            }

            int prevPoints = points;
            points += newPts;
 
            if((prevPoints <50 && points >= 50)) {
                powerups++;
            } 
            
            if((prevPoints < 70 && points >= 70)) {
                powerups++;
            } 

           if (points >= 100)     {
                int currentMilestones = 1 + (points - 100) / 30;
                int previousMilestones = (prevPoints >= 100) ? (1 + (prevPoints - 100) / 30) : 0;
                
                int newMilestones = currentMilestones - previousMilestones;
                powerups += newMilestones;
            }
        }

        for (int i = 0; i < enemyCount; i++) 
            if (grid[a[i].y / ts][a[i].x / ts] == 2) {// if the enemy hits ur trail
                Game = false; //game overrrrrrrrrrr 
                points= 0;
            }

        Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: "+ to_string(points));
        scoreText.setCharacterSize(15);
        scoreText.setFillColor(Color::White);
        scoreText.setPosition(10.f, M*ts + 10.f);

    
        Text powerText;
        powerText.setFont(font);
        powerText.setString("Powerups: "+ to_string(powerups));
        powerText.setCharacterSize(15);
        powerText.setFillColor(Color::White);
        powerText.setPosition(200.f, M*ts+ 10.f);

    
        /////////draw//////////
        
        // Draw background
        if (themeAssets.hasBackground) {
            window.clear();
            window.draw(sBackground);
        } else {
            window.clear(backgroundColor);
        }

        // Draw tiles
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (grid[i][j] == 0)
                    continue;
                
                if (grid[i][j] == 1)
                    sTile.setTextureRect(IntRect(0, 0, ts, ts));  // Solid tile
                if (grid[i][j] == 2)
                    sTile.setTextureRect(IntRect(54, 0, ts, ts)); // Trail tile
                
                sTile.setPosition(j * ts, i * ts);
                window.draw(sTile);

                window.draw(scoreText);
            }

        // Draw player
        sTile.setTextureRect(IntRect(36, 0, ts, ts));  // Player tile
        sTile.setPosition(x * ts, y * ts);
        window.draw(sTile);

        // Rotate and draw enemies
        sEnemy.rotate(10);

        for (int i = 0; i < enemyCount; i++)
        {
            sEnemy.setPosition(a[i].x, a[i].y);
            window.draw(sEnemy);
        }

        if (!Game) {
            window.draw(sGameover);//game overrrr
            points = 0; 
            threshold=10;
            multiplier = 2; 
            rewardCounter = 0;
        }
        window.draw(scoreText);
        window.draw(powerText);
        
        window.display();
    }
   
    return;
}

