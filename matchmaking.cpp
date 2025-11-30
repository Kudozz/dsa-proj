#include"matchmaking.h"
#include"authentication.h"
#include"Player.h"
#include<iostream>
#include<iomanip>
#include<cstdlib>

QueueEntry::QueueEntry(Player*& player, time_t entryTime, bool isBot) {
    this->entryTime = entryTime;
    this->player = player;
    this->isBot = isBot;
}

MatchmakingQueue::MatchmakingQueue(int capacity = 10) {
    this->capacity = capacity;
    heap = new QueueEntry*[capacity];
    size = 0;
}

void MatchmakingQueue::heapify(int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && heap[left]->player->totalScore > heap[largest]->player->totalScore){
        largest = left;
    }

    if (right < size && heap[right]->player->totalScore > heap[largest]->player->totalScore){
        largest = right;
    }

    if (largest != i){
        swap(heap[i], heap[largest]);
        heapify(largest);
    }
}

void MatchmakingQueue::bottomUpHeapify(int i)  {//heapify UP
    int parent = (i - 1) / 2;
    if (parent >= 0)   {
        if (heap[i]->player->totalScore > heap[parent]->player->totalScore)  {
                swap(heap[i], heap[parent]);
                bottomUpHeapify(parent);
            }
        }
}

void MatchmakingQueue::enqueue(Player *player, bool isBot) {
    QueueEntry* temp = new QueueEntry(player, time(0), isBot);

    if(size<capacity) {
        size++;
        heap[size-1] = temp; 
        bottomUpHeapify(size-1);
    } else {
        cout<<"\n\nCannot match you! :( The game room is full!";
    }
}
Player *MatchmakingQueue::dequeue() {
    QueueEntry* temp = heap[0];

    swap(heap[0], heap[size-1]);
    size--;
    heapify(0);

    return temp->player;
}

Player *MatchmakingQueue::peek() {
    return heap[0]->player;
}
int MatchmakingQueue::getSize() {
    return size;
}
void MatchmakingQueue::remove(string username) {
    for(int i=0; i<size; i++) {
        if(heap[i]->player->username == username) {
            swap(heap[i], heap[size-1]);
            size--;
            heapify(i);
            bottomUpHeapify(i);
        }
    }
}
int MatchmakingQueue::getPosition(string username) {
    for(int i=0; i<size; i++) {
        if(heap[i]->player->username == username) 
            return i;
    }
    return -1;
}

void MatchmakingQueue::display() {
    cout << "\n\n\t\t. ݁₊ ⊹ . ݁˖ . ݁. ݁ ˖ ϟ ⚡︎ ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ .\n\n ݁"
         << "\n\t\t\t\t Current Queue \n\n"
         << "\t\t. ݁₊ ⊹ . ݁˖ . ݁. ݁ ˖ ϟ ⚡︎ ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ .\n\n";
    
    cout << setw(10) << "Position" << setw(20) << "Username" 
         << setw(10) << "Score" << setw(15) << "Status\n";
    cout << "-----------------------------------------------------------------\n";
    
    string currentUser = getCurrentUser();
    
    // Display players in priority order (sorted by score)
    for (int i = 0; i <size; i++) {
        QueueEntry* temp = heap[i];
        
        cout << setw(10) << (i + 1) 
             << setw(20) << temp->player->username;
        
        if (temp->player->username == currentUser) {
            cout << " (YOU)";
        }
        
        cout << setw(10) << temp->player->totalScore 
             << setw(15) << "Waiting\n";
    }
    
    cout << "=================================================================\n";
    cout << "Players in queue: " << size << "\n";
    cout << "Minimum for match: 2\n";
    
}
bool MatchmakingQueue::contains(string username) {
    for(int i=0; i<size; i++) {
        if(heap[i]->player->username == username) 
            return true;
    }
    return false;
}

PlayerPool::PlayerPool() {
    size = 15; 
    count = 0;
    players = new Player *[size];
    
}


void PlayerPool::loadFromFile() {
    
}

MatchmakingQueue queue;

void matchmakingMenu() {
    while(true) {
        cout << "\n\n. ݁₊ ⊹ . ݁˖ . ݁. ݁ ˖ ϟ Matchmaking Menu ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ "<<endl
            <<"(1) Enter queue"<<endl
            <<"(2) View queue"<<endl
            <<"(0) Back"<<endl;
        int op;
        cin>>op;
        switch(op) {
            case 0: {
                return;
            }
            case 1: {
                string user = getCurrentUser();
                if(queue.contains(user)) {
                    cout<<"\nYou are already in the queue!";
                    return;
                } else {
                    // Player* currentPlayer = loadPlayer(getCurrentUser());
                   Player* player = new Player("kudoz", 2, 5, 2, 5, 2, 4, 4);
                    queue.enqueue(player, false);
                }
            }
            case 2: {
                queue.display();   
            }
        }
    }
}