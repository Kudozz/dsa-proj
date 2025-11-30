#include "leaderboard.h"
#include"Player.h"
//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME

#include <iostream>
#include <limits>
using namespace std;

const int MAX = 50;
int indexOf[MAX + 1]; // ARRAY TO STORE LOCATION OF EACH VALUE IN THE HEAP


// int Node::count = 0;

class MinHeap{
    Player *arr;
    int size;
    int MAX = 1000;

public:
    MinHeap()    {
        arr = new Player[MAX];
        size = 0;
    }

    void heapify(int i)   {//heapify DOWN
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size &&){
            largest = left;
        }

        if (right < size &&){
            largest = right;
        }

        if (largest != i){
            swap(arr[i], arr[largest]);
            heapify(largest);
        }
    }

    void addToHeap(int val, int freq, int order)    {
        size++;//update heap size
        Player temp(val, freq, order);
        indexOf[val] = size - 1;//update indexOf, tracking where the element is in the heap array
        arr[size - 1] = temp;//add the element to heap

        bottomUpHeapify(size - 1);
    }

    void bottomUpHeapify(int i)  {//heapify UP
        int parent = (i - 1) / 2;
        if (parent >= 0)   {
            if (arr[i].frequency > arr[parent].frequency ||
                (arr[i].frequency == arr[parent].frequency &&
                 arr[i].insertionIndex < arr[parent].insertionIndex))  {//if element has greater frequency AND was inserted first
                    int vi = arr[i].val;
                    int vj = arr[parent].val;

                    swap(arr[i], arr[parent]);

                    indexOf[vi] = parent;//update positions in heap
                    indexOf[vj] = i;
                    bottomUpHeapify(parent);
                }
            }
    }

   
    
};

int main(){
  
    MaxHeap maxheap;            // MAXHEAP TO STORE EACH VALUE ALONG WITH ITS FREQUENCY
    
}
void leaderboard() {
}
