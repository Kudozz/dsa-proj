#include "leaderboard.h"
#include"Player.h"
#include"playerPool.h"
//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME
#include<iomanip>
#include <iostream>
using namespace std;


class MinHeap{
    Player **arr;
    int size;
    int MAX = 10;

public:
    MinHeap()    {
        arr = new Player*[MAX];
        size = 0;
    }

    void heapify(int i)   {//heapify DOWN
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && arr[left]->totalScore < arr[smallest]->totalScore){
            smallest = left;
        }

        if (right < size && arr[right]->totalScore < arr[smallest]->totalScore){
            smallest = right;
        }

        if (smallest != i){
            swap(arr[i], arr[smallest]);
            heapify(smallest);
        }
    }

    void heapifyTemp(Player** temp, int size, int i)   {//heapify DOWN
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && temp[left]->totalScore > temp[largest]->totalScore){
            largest = left;
        }

        if (right < size && temp[right]->totalScore > temp[largest]->totalScore){
            largest = right;
        }

        if (largest != i){
            swap(temp[i], temp[largest]);
            heapifyTemp(temp, size, largest);
        }
    }

    void insert(Player*& player)    {
        size++;//update heap size
        arr[size - 1] = player;//add the element to heap

        bottomUpHeapify(size - 1);
    }

    void bottomUpHeapify(int i)  {//heapify UP
        int parent = (i - 1) / 2;
        if (parent >= 0)   {
            if (arr[i]->totalScore < arr[parent]->totalScore)  {
                    swap(arr[i], arr[parent]);
                    bottomUpHeapify(parent);
                }
            }
    }
    
    void updateLeaderboard(Player*& player) {
        if(size<10) {
            insert(player);
        } else if(size == 10) {
            if(arr[0]->totalScore < player->totalScore) {
                arr[0] = player; 
                heapify(0);
            }
        }
    }

    void displayLeaderboard(){

        Player **temp = new Player*[size];
        for (int i = 0; i < size; i++)
            temp[i] = arr[i];

        for (int i = (size / 2) - 1; i >= 0; i--)
            heapifyTemp(temp, size, i);

        //heapsortttttt
        for (int i = size - 1; i > 0; i--) {
            // Move current root to end
            swap(temp[0], temp[i]);

            // Call max heapify on the reduced heap
            heapifyTemp(temp, i, 0);
        }


        cout << "\n\n\t\t. ݁₊ ⊹ . ݁˖ . ݁. ݁ ˖ ϟ ⚡︎ ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ .\n\n ݁"
            <<"\n\t\t\t\tLeaderboard \n\n"
            <<"\t\t. ݁₊ ⊹ . ݁˖ . ݁. ݁ ˖ ϟ ⚡︎ ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ .\n\n";

        for(int i=size-1; i>=0; i--) {
            cout << "\n\t\t" << size-i << setw(20) << temp[i]->username << setw(20) << temp[i]->totalScore << endl
                 << ". ݁₊ ⊹ . ݁˖ . ݁. ݁ ˖ ϟ ⚡︎ ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ .. ݁₊ ⊹ . ݁˖ . ݁. ݁ ˖ ϟ ⚡︎ ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ .";
        }

    }

    ~MinHeap() {
        for(int i=0; i<size; i++)
            delete arr[i];

        delete arr;
    }
};

void leaderboard(){
   MinHeap board;
   PlayerPool playerpool; 
   playerpool.loadFromFile();

   for(int i=0; i<playerpool.size; i++) {
    board.updateLeaderboard(playerpool.players[i]);
   }
//    Player* temp = new Player("kudoz", 45, 4500, 56, 4, 6, 3, 6);
//    board.updateLeaderboard(temp);
//    Player *temp2 = new Player("wajiha", 45, 457, 56, 4, 6, 3, 6);
//    board.updateLeaderboard(temp2);


//     Player *temp3 = new Player("wajiha", 45, 356, 56, 4, 6, 3, 6);
//    board.updateLeaderboard(temp3);

//    Player *temp4 = new Player("harma", 45, 34, 56, 4, 6, 3, 6);
//    board.updateLeaderboard(temp4);

//    Player *temp5 = new Player("uswa", 45, 760, 56, 4, 6, 3, 6);
//    board.updateLeaderboard(temp5);
//    Player *temp6 = new Player("zaytunah", 45, 120, 56, 4, 6, 3, 6);
//    board.updateLeaderboard(temp6);
//    Player *temp7 = new Player("haadiya", 45, 30, 56, 4, 6, 3, 6);
//    board.updateLeaderboard(temp7);
//    Player *temp8 = new Player("maryam", 45, 500, 56, 4, 6, 3, 6);
//    board.updateLeaderboard(temp8);
//    Player *temp9 = new Player("ayesha", 45, 20, 56, 4, 6, 3, 6);
//    board.updateLeaderboard(temp9);
//    Player *temp10 = new Player("musa bhai", 45, 100, 56, 4, 6, 3, 6);
//    board.updateLeaderboard(temp10);

//    Player *temp11 = new Player("mam naveen", 45, 5600, 56, 4, 6, 3, 6);
//    board.updateLeaderboard(temp11);

   board.displayLeaderboard();

}
