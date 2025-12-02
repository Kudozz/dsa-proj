#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
using namespace std;

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME

const int GRID_N = 48;  // Match your N
const int GRID_M = 32;  // Match your M

// Node for linked list of grid tiles
struct TileNode {
    int x;              // X coordinate in grid
    int y;              // Y coordinate in grid
    int value;          // Grid value (0=empty, 1=solid, 2=trail, -1=enemy zone)
    TileNode* next;

    TileNode(int xPos, int yPos, int val) {
        x = xPos;
        y = yPos;
        value = val;
        next = nullptr;
    }
};

// Enemy position structure
struct EnemyData {
    float x;            // Enemy X position (pixel)
    float y;            // Enemy Y position (pixel)
    float dx;           // Enemy X velocity
    float dy;           // Enemy Y velocity
};

// GameState structure for XONIX
struct GameState {
    string saveID;          // Unique save identifier
    string username;        // Player username
    int playerID;           // Player ID
    time_t timestamp;       // Save timestamp
    
    // Player data
    int playerX;            // Player grid X
    int playerY;            // Player grid Y
    int playerDX;           // Player direction X
    int playerDY;           // Player direction Y
    
    // Game stats
    int score;              // Current score
    int points;             // Points earned
    int powerups;           // Powerups available
    int enemyCount;         // Number of enemies
    int mode;               // Game mode (1=easy, 2=medium, 3=hard)
    
    // Multiplier system
    int multiplier;
    int threshold;
    int rewardCounter;
    
    // Enemy data
    EnemyData enemies[10];  // Store enemy positions
    
    // Grid data (linked list)
    TileNode* tilesHead;    // Linked list of non-zero tiles
    int tileCount;          // Number of tiles in list

    GameState() {
        saveID = "";
        username = "";
        playerID = 0;
        timestamp = 0;
        playerX = 0;
        playerY = 0;
        playerDX = 0;
        playerDY = 0;
        score = 0;
        points = 0;
        powerups = 0;
        enemyCount = 4;
        mode = 1;
        multiplier = 2;
        threshold = 10;
        rewardCounter = 0;
        tilesHead = nullptr;
        tileCount = 0;
        
        for(int i = 0; i < 10; i++) {
            enemies[i].x = 0;
            enemies[i].y = 0;
            enemies[i].dx = 0;
            enemies[i].dy = 0;
        }
    }

    ~GameState() {
        // Clean up linked list
        TileNode* current = tilesHead;
        while (current) {
            TileNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    // Add a tile to the linked list (only non-zero tiles)
    void addTile(int x, int y, int value) {
        if (value == 0) return;  // Don't store empty tiles
        
        TileNode* newNode = new TileNode(x, y, value);
        newNode->next = tilesHead;
        tilesHead = newNode;
        tileCount++;
    }
};

// SaveGame Manager Class
class SaveGameManager {
private:
    string generateSaveID(const string& username);
    string getTimestamp(time_t time);

public:
    SaveGameManager();
    ~SaveGameManager();

    // Save game state to file
    bool saveGame(GameState* state, const string& username, int playerID);
    
    // Load game state from file using Save ID
    GameState* loadGame(const string& saveID);
    
    // Check if user has a saved game
    bool hasSavedGame(const string& username);
    
    // Get the most recent save for a user
    string getMostRecentSave(const string& username);
    
    // Delete a save file
    bool deleteSave(const string& saveID);
    
    // Check if save exists
    bool saveExists(const string& saveID);
};

#endif