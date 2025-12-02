#include "SaveGame.h"
#include "SystemLogs.h"
#include <ctime>
#include <cstring>

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME

SaveGameManager::SaveGameManager() {
    // Constructor
}

SaveGameManager::~SaveGameManager() {
    // Destructor
}

// Generate unique Save ID: USERNAME_TIMESTAMP
string SaveGameManager::generateSaveID(const string& username) {
    time_t now = time(0);
    string saveID = username + "_" + to_string(now);
    return saveID;
}

// Convert timestamp to readable string
string SaveGameManager::getTimestamp(time_t time) {
    char buffer[80];
    struct tm* timeinfo = localtime(&time);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}

// Save game state to file
bool SaveGameManager::saveGame(GameState* state, const string& username, int playerID) {
    if (!state) {
        cout << "Error: Invalid game state!\n";
        return false;
    }

    // Generate unique Save ID
    state->saveID = generateSaveID(username);
    state->username = username;
    state->playerID = playerID;
    state->timestamp = time(0);

    string filename = "profiles/" + state->saveID + ".sav";
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not create save file!\n";
        return false;
    }

    // Write metadata
    file << "SAVE_ID:" << state->saveID << endl;
    file << "USERNAME:" << state->username << endl;
    file << "PLAYER_ID:" << state->playerID << endl;
    file << "TIMESTAMP:" << state->timestamp << endl;
    file << "PLAYER_X:" << state->playerX << endl;
    file << "PLAYER_Y:" << state->playerY << endl;
    file << "PLAYER_DX:" << state->playerDX << endl;
    file << "PLAYER_DY:" << state->playerDY << endl;
    file << "SCORE:" << state->score << endl;
    file << "POINTS:" << state->points << endl;
    file << "POWERUPS:" << state->powerups << endl;
    file << "ENEMY_COUNT:" << state->enemyCount << endl;
    file << "MODE:" << state->mode << endl;
    file << "MULTIPLIER:" << state->multiplier << endl;
    file << "THRESHOLD:" << state->threshold << endl;
    file << "REWARD_COUNTER:" << state->rewardCounter << endl;
    file << "TILE_COUNT:" << state->tileCount << endl;

    // Write enemy data
    file << "ENEMIES_BEGIN" << endl;
    for (int i = 0; i < state->enemyCount; i++) {
        file << state->enemies[i].x << " " 
             << state->enemies[i].y << " "
             << state->enemies[i].dx << " "
             << state->enemies[i].dy << endl;
    }
    file << "ENEMIES_END" << endl;

    // Write tiles linked list
    file << "TILES_BEGIN" << endl;
    TileNode* current = state->tilesHead;
    while (current) {
        file << current->x << " " 
             << current->y << " " 
             << current->value << endl;
        current = current->next;
    }
    file << "TILES_END" << endl;

    file.close();

    cout << "\n⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ Game Saved Successfully! ⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";
    cout << "Save ID: " << state->saveID << endl;
    cout << "Saved at: " << getTimestamp(state->timestamp) << endl;
    cout << "⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";

    logSysActivity(username, "Game saved - " + state->saveID, "SUCCESS");
    return true;
}

// Load game state from file
GameState* SaveGameManager::loadGame(const string& saveID) {
    string filename = "profiles/" + saveID + ".sav";
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Save file not found!\n";
        return nullptr;
    }

    GameState* state = new GameState();
    string line;
    string tag;
    bool readingEnemies = false;
    bool readingTiles = false;
    int enemyIndex = 0;

    while (getline(file, line)) {
        if (line == "ENEMIES_BEGIN") {
            readingEnemies = true;
            enemyIndex = 0;
            continue;
        }
        
        if (line == "ENEMIES_END") {
            readingEnemies = false;
            continue;
        }
        
        if (line == "TILES_BEGIN") {
            readingTiles = true;
            continue;
        }
        
        if (line == "TILES_END") {
            readingTiles = false;
            continue;
        }

        if (readingEnemies) {
            // Parse enemy data: x y dx dy
            float x, y, dx, dy;
            size_t pos1 = 0, pos2 = 0;
            
            pos2 = line.find(' ', pos1);
            x = stof(line.substr(pos1, pos2 - pos1));
            pos1 = pos2 + 1;
            
            pos2 = line.find(' ', pos1);
            y = stof(line.substr(pos1, pos2 - pos1));
            pos1 = pos2 + 1;
            
            pos2 = line.find(' ', pos1);
            dx = stof(line.substr(pos1, pos2 - pos1));
            pos1 = pos2 + 1;
            
            dy = stof(line.substr(pos1));
            
            state->enemies[enemyIndex].x = x;
            state->enemies[enemyIndex].y = y;
            state->enemies[enemyIndex].dx = dx;
            state->enemies[enemyIndex].dy = dy;
            enemyIndex++;
            
        } else if (readingTiles) {
            // Parse tile data: x y value
            int x, y, value;
            size_t pos1 = 0, pos2 = 0;
            
            pos2 = line.find(' ', pos1);
            x = stoi(line.substr(pos1, pos2 - pos1));
            pos1 = pos2 + 1;
            
            pos2 = line.find(' ', pos1);
            y = stoi(line.substr(pos1, pos2 - pos1));
            pos1 = pos2 + 1;
            
            value = stoi(line.substr(pos1));
            
            state->addTile(x, y, value);
            
        } else {
            // Parse metadata
            size_t colonPos = line.find(':');
            if (colonPos > 0 && colonPos < line.length()) {
                tag = line.substr(0, colonPos);
                string value = line.substr(colonPos + 1);

                if (tag == "SAVE_ID") {
                    state->saveID = value;
                } else if (tag == "USERNAME") {
                    state->username = value;
                } else if (tag == "PLAYER_ID") {
                    state->playerID = stoi(value);
                } else if (tag == "TIMESTAMP") {
                    state->timestamp = stol(value);
                } else if (tag == "PLAYER_X") {
                    state->playerX = stoi(value);
                } else if (tag == "PLAYER_Y") {
                    state->playerY = stoi(value);
                } else if (tag == "PLAYER_DX") {
                    state->playerDX = stoi(value);
                } else if (tag == "PLAYER_DY") {
                    state->playerDY = stoi(value);
                } else if (tag == "SCORE") {
                    state->score = stoi(value);
                } else if (tag == "POINTS") {
                    state->points = stoi(value);
                } else if (tag == "POWERUPS") {
                    state->powerups = stoi(value);
                } else if (tag == "ENEMY_COUNT") {
                    state->enemyCount = stoi(value);
                } else if (tag == "MODE") {
                    state->mode = stoi(value);
                } else if (tag == "MULTIPLIER") {
                    state->multiplier = stoi(value);
                } else if (tag == "THRESHOLD") {
                    state->threshold = stoi(value);
                } else if (tag == "REWARD_COUNTER") {
                    state->rewardCounter = stoi(value);
                }
            }
        }
    }

    file.close();

    cout << "\n⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ Game Loaded Successfully! ⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";
    cout << "Player: " << state->username << endl;
    cout << "Saved at: " << getTimestamp(state->timestamp) << endl;
    cout << "Score: " << state->points << " | Powerups: " << state->powerups << endl;
    cout << "⋆. 𐙚 ̊ ✦•┈๑⋅⋯⋆ ˚⋆୨♡୧⋆ ˚⋆⋯⋅๑┈•✦⋆. 𐙚 ̊\n";

    logSysActivity(state->username, "Game loaded - " + saveID, "SUCCESS");
    return state;
}

// Check if user has any saved game
bool SaveGameManager::hasSavedGame(const string& username) {
    string recentSave = getMostRecentSave(username);
    return !recentSave.empty();
}

// Get most recent save for user
string SaveGameManager::getMostRecentSave(const string& username) {
    // Simple implementation: check for files starting with username_
    // In production, you'd scan the directory
    
    // Try recent timestamps (last 30 days)
    time_t now = time(0);
    for (int days = 0; days < 30; days++) {
        time_t checkTime = now - (days * 24 * 3600);
        string saveID = username + "_" + to_string(checkTime);
        
        if (saveExists(saveID)) {
            return saveID;
        }
    }
    
    return "";  // No save found
}

// Delete a save file
bool SaveGameManager::deleteSave(const string& saveID) {
    string filename = "profiles/" + saveID + ".sav";
    
    if (remove(filename.c_str()) == 0) {
        return true;
    }
    return false;
}

// Check if save exists
bool SaveGameManager::saveExists(const string& saveID) {
    string filename = "profiles/" + saveID + ".sav";
    ifstream file(filename);
    bool exists = file.is_open();
    file.close();
    return exists;
}