#include "leaderboard.h"

//Wajiha Abbasi 24i-2059
//Hanaa Sajid  24i-2029
//PROJECT: XONIX GAME

#include <iostream>
#include <limits>
using namespace std;

const int MAX = 50;
int indexOf[MAX + 1]; // ARRAY TO STORE LOCATION OF EACH VALUE IN THE HEAP

struct Node{
public:
    int val;
    int frequency;
    int insertionIndex;
    // static int count;
    Node(int v = 0, int f = 0, int ind = 0)
    {
        val = v;
        insertionIndex = ind;
        frequency = f;
    }
};

// int Node::count = 0;

class Queue{
    int maxSize;
    int *q;
    int front, rear;

public:
    Queue(int size){//constructor
        maxSize = size;
        front = rear = -1;
        q = new int[maxSize];
    }

    void enqueue(int x) {//enqueue (circular)
        if ((rear + 1) % maxSize == front){
            return;
        }else if (rear == -1){
            front = rear = 0;
            q[rear] = x;
            return;
        }  else {
            rear = (rear + 1) % maxSize;
            q[rear] = x;
        }
    }

    int dequeue() {//dequeue (circular)
        if (rear == -1)   {
            return -999999;
        }    else if (front == rear) {
            int x = q[front];
            front = rear = -1;
            return x;
        } else    {
            int x = q[front];
            front = (front + 1) % maxSize;
            return x;
        }
    }

    bool isEmpty()    {
        return (front == -1 && rear == -1);
    }

    bool isFull()    {
        return ((rear + 1) % maxSize == front);
    }

    void display()    {//circular display
        cout << "[ ";
        if (front != -1)        {
            int i = front;

            while (true)    {
                cout << q[i] << " ";
                if (i == rear) {
                    break;
                }
                i = (i + 1) % maxSize;
            }
        }
        cout << "]";
    }
};

class MaxHeap{
    Node *arr;
    int size;
    int MAX = 1000;

public:
    MaxHeap()    {
        arr = new Node[MAX];
        size = 0;
    }

    void heapify(int i)   {//heapify DOWN
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size &&
            (arr[left].frequency > arr[largest].frequency ||
             (arr[left].frequency == arr[largest].frequency &&
              arr[left].insertionIndex < arr[largest].insertionIndex)))
        {
            largest = left;
        }

        if (right < size &&
            (arr[right].frequency > arr[largest].frequency ||
             (arr[right].frequency == arr[largest].frequency &&
              arr[right].insertionIndex < arr[largest].insertionIndex))){
            largest = right;
        }

        if (largest != i){
            int vi = arr[i].val;
            int vj = arr[largest].val;

            swap(arr[i], arr[largest]);

            indexOf[vi] = largest;
            indexOf[vj] = i;
            heapify(largest);
        }
    }

    void addToHeap(int val, int freq, int order)    {
        size++;//update heap size
        Node temp(val, freq, order);
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
