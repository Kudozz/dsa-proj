#include "inventory.h"
#include "Player.h"
#include "authentication.h"
#include <iostream>
#include <fstream>

using namespace std;


//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME

Inventory::Inventory(){
    root = nullptr;
    currentThemeID = 1;    //default theme

}

Inventory::~Inventory(){
    destroyTree(root);
}

void Inventory::destroyTree(AVLNode* node){
    if(node == nullptr){
        return;
    }

    destroyTree(node->left);
    destroyTree(node->right);

    delete node;
}

int Inventory::getHeight(AVLNode* node){
    if(node == nullptr){
        return 0;
    }

    return (node->height);
}

int Inventory::getBalance(AVLNode* node){
    if(node == nullptr){
        return 0;
    }

    return (getHeight(node->left)- getHeight(node->right));
}

AVLNode* Inventory::rightRotate(AVLNode* y){
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    y->height = 1 +(getHeight(y->left)> getHeight(y->right)? getHeight(y->left): getHeight(y->right));
    x->height = 1 +(getHeight(x->left)> getHeight(x->right)? getHeight(x->left): getHeight(x->right));
    
    return x;
}

AVLNode* Inventory::leftRotate(AVLNode* x){
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    x->height = 1 +(getHeight(x->left)> getHeight(x->right)? getHeight(x->left): getHeight(x->right));
    y->height = 1 +(getHeight(y->left)> getHeight(y->right)? getHeight(y->left): getHeight(y->right));
    
    return y;
}

AVLNode* Inventory::insert(AVLNode* node,Theme theme){
    if(node == nullptr){
        return new AVLNode(theme);
    }
    
    if(theme.themeID < node->theme.themeID){
        node->left = insert(node->left,theme);
    } 
    
    else if(theme.themeID > node->theme.themeID){
        node->right = insert(node->right,theme);
    } 
    
    else{
        return node;
    }
    
    node->height = 1 +(getHeight(node->left)> getHeight(node->right)? getHeight(node->left): getHeight(node->right));
    
    int balance= getBalance(node);
    
    if((balance>1) && (theme.themeID< node->left->theme.themeID)){
        return rightRotate(node);

    }
    
    if((balance< -1) && (theme.themeID> node->right->theme.themeID)){

        return leftRotate(node);
    }
    
    if((balance>1) && (theme.themeID> node->left->theme.themeID)){
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    if((balance< -1) && (theme.themeID< node->right->theme.themeID)){
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    
    return node;
}

AVLNode* Inventory::search(AVLNode* node, int themeID){
    if((node==nullptr)||(node->theme.themeID== themeID)){
        return node;
    }
    
    if(themeID< node->theme.themeID){
        return search(node->left,themeID);
    }
    
    return search(node->right,themeID);
}


void Inventory::displayTheme(const Theme& theme){
    cout<< "\n\tજ⁀➴ ♡                         ִֶָ  𓂃 ࣪ ִֶָ🦢་༘࿐"<<endl;
    cout<< "\t   Theme ID: "<<theme.themeID<<endl;
    cout<< "\t  Name: "<<theme.themeName<<endl;
    cout<< "\t  Description: " << theme.description<<endl;
    cout<< "\t｡⋆𖦹.✧˚ ⋆. 𐙚 ̊  ⋆ ˚｡ ⋆୨♡୧⋆｡˚ ⋆ ˚ 𐙚 .⋆ ༘˚⋆｡⋆𖦹.✧˚"<<endl;
}

void Inventory::inOrderTraversal(AVLNode* node){
    if(node== nullptr){
        return;
    }
    
    inOrderTraversal(node->left);
    displayTheme(node->theme);
    inOrderTraversal(node->right);
}

void Inventory::addTheme(int id,string name,string desc, string color,bool useTextures){
    Theme newTheme(id,name,desc,color,useTextures);

    root = insert(root, newTheme);
}

Theme* Inventory::findTheme(int themeID){
    AVLNode* result = search(root, themeID);
    if(result == nullptr){
        return nullptr;
    }

    return (&(result->theme));
}

void Inventory::displayAllThemes(){
    cout<<"\n\n\t⋆｡‧˚ʚ♡ɞ˚‧｡⋆ AVAILABLE THEMES ⋆｡‧˚ʚ♡ɞ˚‧｡⋆\n"<<endl;
    if(root == nullptr){
        cout<<"\t\tNo themes available!"<<endl;

        return;

    }

    inOrderTraversal(root);

}

void Inventory::selectTheme(int themeID){
    Theme* theme = findTheme(themeID);
    if(theme!=nullptr){

        currentThemeID = themeID;
        
        cout<<"\n\tTheme '"<<theme->themeName<<"' selected!"<<endl;
        Player* player= getCurrentPlayer();

        if(player){
            player->currentThemeID =themeID;
        }
        saveThemePreference(getCurrentUser());

    } 
    
    else{
        cout<<"\n\ttheme not found!"<<endl;
    }
}

int Inventory::getCurrentTheme(){
    Player* player= getCurrentPlayer();

    
    return ((player)?player->currentThemeID: currentThemeID);
}

void Inventory::saveThemePreference(const string& username){
    string filename = username +"_theme.txt";

    ofstream file(filename.c_str());
    
    if(file.is_open()){

        file<<currentThemeID<<endl;
        file.close();
    }

}

void Inventory::loadThemePreference(const string& username){
    string filename = username+"_theme.txt";
    
    ifstream file(filename.c_str());
    
    if(file.is_open()){

        file>>currentThemeID;
        file.close();
    } 
    
    else{
        currentThemeID = 1;
    }

}

ThemeColors Inventory::getThemeColors(int themeID){
    ThemeColors colors;
    
    switch(themeID){
        case 2:{//blue-hana
            colors.r_primary = 25;
            colors.g_primary = 25;
            colors.b_primary = 112;

            colors.r_trail = 100;
            colors.g_trail = 149;
            colors.b_trail = 237;

            colors.r_player = 135;
            colors.g_player = 206;
            colors.b_player = 250;

            colors.r_background = 0;
            colors.g_background = 0;
            colors.b_background = 30;
            break;

        }

        case 3:{//pink
            colors.r_primary = 255;
            colors.g_primary = 182;
            colors.b_primary = 193;

            colors.r_trail = 255;
            colors.g_trail = 192;
            colors.b_trail = 203;

            colors.r_player = 255;
            colors.g_player = 105;
            colors.b_player = 180;

            colors.r_background = 255;
            colors.g_background = 240;
            colors.b_background = 245;

            break;

        }
        
        case 4:{//galaxy
            colors.r_primary = 25;
            colors.g_primary = 25;
            colors.b_primary = 112;


            colors.r_trail = 138;
            colors.g_trail = 43;
            colors.b_trail = 226;

            colors.r_player = 255;
            colors.g_player = 215;
            colors.b_player = 0;

            colors.r_background = 0;
            colors.g_background = 0;
            colors.b_background = 0;

            break;

        }

        case 5:{//unicornn
            colors.r_primary = 255;
            colors.g_primary = 182;
            colors.b_primary = 193;

            colors.r_trail = 230;
            colors.g_trail = 190;
            colors.b_trail = 255;

            colors.r_player = 255;
            colors.g_player = 105;
            colors.b_player = 180;

            colors.r_background = 255;
            colors.g_background = 240;
            colors.b_background = 245;

            break;

        }


        case 6:{//jungle
            colors.r_primary = 34;
            colors.g_primary = 139;
            colors.b_primary = 34;

            colors.r_trail = 144;
            colors.g_trail = 238;
            colors.b_trail = 144;

            colors.r_player = 218;
            colors.g_player = 165;
            colors.b_player = 32;

            colors.r_background = 27;
            colors.g_background = 77;
            colors.b_background = 62;

            break;

        }

        default:{
            colors.r_primary = 0;
            colors.g_primary = 102;
            colors.b_primary = 204;

            colors.r_trail = 100;
            colors.g_trail = 200;
            colors.b_trail = 255;

            colors.r_player = 255;
            colors.g_player = 100;
            colors.b_player = 100;

            colors.r_background = 240;
            colors.g_background = 248;
            colors.b_background = 255;
            break;

        }

    }
    
    return colors;
}


ThemeAssets Inventory::getThemeAssets(int themeID){
    ThemeAssets assets;
    
    switch(themeID){
        case 1:{//default
            assets.tileTexture="images/tiles.png";
            assets.enemyTexture="images/enemy.png";
            assets.hasBackground=false;

            break;
        
        }


        case 2:{//blue-hanaa
            assets.tileTexture="images/blue_tile.png";
            assets.enemyTexture="images/enemy.png";
            assets.hasBackground=false;

            break;
        }

        case 3:{//pink/purple
            assets.tileTexture="images/purple.png";
            assets.enemyTexture="images/enemy.png";
            assets.hasBackground=false;
            break;
        }

        case 4:{//galaxy
            assets.tileTexture="images/purple.png";
            assets.enemyTexture="images/star.png";
            assets.backgroundTexture="images/galaxy_bg.png";
            assets.hasBackground=true;
            break;
        }

        case 5:{//unicorn
            assets.tileTexture="images/purple.png";
            assets.enemyTexture="images/unicorn.png";
            assets.backgroundTexture="images/sparkly_bg.png";
            assets.hasBackground=true;

            break;
        }

        case 6:{//jungle
            assets.tileTexture="images/green_tile.png";
            assets.enemyTexture="images/lion.png";
            assets.backgroundTexture="images/jungle_bg.png";
            assets.hasBackground=true;

            break;
        }
        
        default:{
            assets.tileTexture="images/tiles.png";
            assets.enemyTexture="images/enemy.png";
            assets.hasBackground=false;
            break;

        }
    }
    
    return assets;
}


void Inventory::initializeDefaultThemes() {

    addTheme(1,"Default","Original game theme with textures","#ORIGINAL",true);
    addTheme(2,"Blue","Dark blue theme","#191970",false);
    addTheme(3,"Pink/Purple","Gentle pink theme","#FFB6C1",false);
    addTheme(4,"Galaxy","Cosmic space theme with stars","#191970",false);
    addTheme(5,"Unicorn ","Magical sparkly unicorn theme","#FFB6C1",false);
    addTheme(6,"Jungle","Wild jungle adventure theme","#228B22",false);

}

void Inventory::inventoryMenu(){
    
    while(true){
        cout<<"\n\n\t⋆｡‧˚ʚ♡ɞ˚‧｡⋆ INVENTORY MENU ⋆｡‧˚ʚ♡ɞ˚‧｡⋆"<<endl;
        cout<<"\n\t(1) View All Themes"<<endl;
        cout<<"\t(2) Search Theme by ID"<<endl;
        cout<<"\t(3) Select Theme"<<endl;
        cout<<"\t(4) View Current Theme"<<endl;
        cout<<"\t(0) Back to Main Menu"<<endl;
        cout<<"\n\tEnter choice: ";
        
        int choice;
        cin>>choice;
        
        switch(choice){
            case 0:
                return;

            case 1:
                displayAllThemes();
                break;

            case 2:{
                int id;
                cout<<"\n\tEnter Theme ID: ";
                cin>>id;
                
                Theme* theme = findTheme(id);
                
                if(theme!=nullptr){
                    displayTheme(*theme);
                } 
                
                else{
                    cout<<"\n\t Theme not found!"<<endl;
                }

                break;

            }

            case 3:{
                int id;
                cout<<"\n\tEnter Theme ID to select: ";
                cin>>id;
                selectTheme(id);
                
                break;
            }

            case 4:{
                Theme* theme = findTheme(getCurrentTheme());
                
                if(theme!=nullptr){
                    cout<<"\n\t Current Theme:"<<endl;
                    displayTheme(*theme);

                } 
                
                else{
                    cout<<"\n\tno theme selected!"<<endl;
                }

                break;
            }

            default:
                cout<<"\n\t Invalid choice!"<<endl;

                break;
        }

    }
}

static Inventory gameInventory;

void inventory(){
    if(isUserLoggedIn()){
        gameInventory.loadThemePreference(getCurrentUser());
    }
    
    gameInventory.inventoryMenu();
}

void initializeInventory(){
    gameInventory.initializeDefaultThemes();
}

Inventory& getInventoryInstance(){
    return gameInventory;
}