#ifndef INVENTORY_H
#define INVENTORY_H


//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME


#include <string>
using namespace std;

struct ThemeColors{
    int r_primary, g_primary, b_primary;
    int r_trail, g_trail, b_trail;
    int r_player, g_player, b_player;
    int r_background, g_background, b_background;
    
    ThemeColors(): r_primary(0), g_primary(0),b_primary(0),
                r_trail(0), g_trail(0),b_trail(0),
                r_player(0), g_player(0),b_player(0),
                r_background(0), g_background(0),b_background(0){}

};

struct ThemeAssets{
    string tileTexture;
    string enemyTexture;
    string backgroundTexture;
    bool hasBackground;
    
    ThemeAssets(): tileTexture(""), enemyTexture(""), backgroundTexture(""), hasBackground(false){}

};

struct Theme{
    int themeID;
    string themeName;
    string description;
    string colorCode;
    bool useTextures;
    
    Theme(): themeID(0),themeName(""),description(""),colorCode(""),useTextures(false){}

    Theme(int id,string name,string desc,string color,bool textures= false)
    :themeID(id),themeName(name),description(desc),colorCode(color),useTextures(textures){}

};


struct AVLNode{
    Theme theme;
    AVLNode* left;
    AVLNode* right;
    int height;
    
    AVLNode(Theme t){
        theme=t;
        left =nullptr;
        right=nullptr;
        height=1;

    }

};

class Inventory{
private:

    AVLNode* root;
    int currentThemeID;
    
    int getHeight(AVLNode* node);
    int getBalance(AVLNode* node);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insert(AVLNode* node,Theme theme);
    AVLNode* search(AVLNode* node,int themeID);
    void inOrderTraversal(AVLNode* node);
    void displayTheme(const Theme&theme);
    void destroyTree(AVLNode* node);
    
public:
    Inventory();
    ~Inventory();
    
    void initializeDefaultThemes();
    void addTheme(int id,string name,string desc,string color,bool useTextures=false);
    Theme* findTheme(int themeID);
    void displayAllThemes();
    void selectTheme(int themeID);
    int getCurrentTheme();
    void saveThemePreference(const string& username);
    void loadThemePreference(const string& username);
    void inventoryMenu();
    ThemeColors getThemeColors(int themeID);
    ThemeAssets getThemeAssets(int themeID);

};


void inventory();
void initializeInventory();
Inventory& getInventoryInstance();

#endif