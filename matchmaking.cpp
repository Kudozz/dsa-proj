#include"matchmaking.h"
#include"authentication.h"
#include"Player.h"
#include"multiPlayer.h"
#include<fstream>
#include<iostream>
#include<iomanip>
#include<cstdlib>

QueueEntry::QueueEntry(Player*& player, time_t entryTime, bool isBot) {
    this->entryTime = entryTime;
    this->player = player;
    this->isBot = isBot;
}

MatchmakingQueue::MatchmakingQueue(int capacity) {
    this->capacity = capacity;
    heap = new QueueEntry*[capacity];
    size = 0;

    playerPool = new PlayerPool;
    playerPool->loadFromFile();
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
    Player* player = temp->player;

    swap(heap[0], heap[size-1]);
    size--;
    heapify(0);

    delete temp;

    return player;
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

            if(i == 0)
                heapify(0);
            else if(heap[i]->player->totalScore > heap[(i-1)/2]->player->totalScore)
                bottomUpHeapify(i);
            else
                heapify(i);
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
    QueueEntry** tempArr = new QueueEntry*[size];
    for(int i=0; i<size; i++) {
        tempArr[i] = heap[i];
    }

    for (int i = 0; i < size; i++) {
    for (int j = i + 1; j < size; j++) {
        if (tempArr[j]->player->totalScore > tempArr[i]->player->totalScore) {
            swap(tempArr[i], tempArr[j]);
        }
    }
}

    cout << "\n\n\t\t. ݁₊ ⊹ . ݁˖ . ݁. ݁ ˖ ϟ ⚡︎ ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ .\n\n ݁"
         << "\n\t\t\t\t Current Queue \n\n"
         << "\t\t. ݁₊ ⊹ . ݁˖ . ݁. ݁ ˖ ϟ ⚡︎ ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ .\n\n";
    
    cout << setw(10) << "Position" << setw(20) << "Username" 
         << setw(10) << "Score" << setw(15) << "Status\n";
    cout << "-----------------------------------------------------------------\n";
    
    string currentUser = getCurrentUser();
    
    // Display players in priority order (sorted by score)
    for (int i = 0; i <size; i++) {
        QueueEntry* temp = tempArr[i];
        
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
    
    delete[] tempArr;
}
bool MatchmakingQueue::contains(string username) {
    for(int i=0; i<size; i++) {
        if(heap[i]->player->username == username) 
            return true;
    }
    return false;
}

void MatchmakingQueue::waitInQueue(Player* player) {
    int delay = (rand() % 3) + 2;

    time_t lastAddTime = time(0);

    bool userInQueue = true;
    while(userInQueue) {
        if(difftime(time(0), lastAddTime) >= delay) {
            Player* randomPlayer = playerPool->getRandomPlayer();
            if(randomPlayer!= nullptr) {
                enqueue(randomPlayer, true);
                cout<<"\n"<<randomPlayer->username<<" joined the queue";
            }
            delay = (rand()%2)+4;
            //timer

            display();
            lastAddTime = time(0);
        }

        if(size >=2) {
            matchPlayers();
            if(!contains(getCurrentUser()))
                userInQueue = false;
        }
    }
}

void MatchmakingQueue::matchPlayers() {
    Player* player1 = dequeue();
    Player* player2 = dequeue();

    if(player1->username == getCurrentUser() || player2->username == getCurrentUser()) {
        cout << "\n\n\t\t . ݁₊ ⊹ . ݁˖ . ݁. ݁ ˖ ϟ Match Found ⚡︎ ϟ ˖ ݁ .. ݁₊ ⊹ . ݁˖ . ݁"
            <<setw(30)<<"\n PLAYER 1"<<setw(20)<< "VS."<<setw(30)<< " PLAYER 2";

        cout << setw(30) << player1->username << setw(20) << "VS." << setw(30) << player2->username;
        multiPlayer();
        //multiplayer function call
    } else {
        cout<<"\n"<<player1->username<<" and "<<player2->username<<" are matched for a game!";
    }
    return;
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
                    Player* currentPlayer = getCurrentPlayer();
                    queue.enqueue(currentPlayer, false);
                    queue.waitInQueue(currentPlayer);
                }
                break;
            }
            case 2: {
                queue.display();   
                break;
            }

            default: {
                cout<<"\nIncorrect option";
                continue;
            }
        }
    }
}