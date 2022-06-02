#include<bits/stdc++.h>
using namespace std;

class Node{
    public:
    Node* prev;
    int value;
    Node* next;

    Node(int value){
        this->value = value;
        prev = NULL;
        next = NULL;
    }
};

class LinkedList{
    Node* head;
    int length;

    public:
    LinkedList();
    void insertFront(int);
    //void remove(int i);
    Node* search(int i);
    int findIndex(int i);
    bool remove(int);
    int size();
    int print();
};

LinkedList::LinkedList(){
    head = NULL;
    length = 0;
}

void LinkedList::insertFront(int value){
    Node* n = new Node(value);
    length++;

    if(head == NULL){
        head = n;
    }else{
        n->next = head; // append head to new node
        head->prev = n;
        head = n; // new node is the new head
    }
}

/**
 * @brief Linearly search the value 
 * from start and return the Node pointer 
 * of the 1st occurance if found.
 * 
 * @param value 
 * @return Node* if found else return NULL
 */
Node* LinkedList::search(int value){
    //cout<<"searching "<<value<<endl;
    Node* temp = head;

    while(temp != NULL){
        //cout<<temp->value <<" ";
        if(temp->value == value)
            return temp;
        temp = temp->next;
    }
    return temp;
}

int LinkedList::findIndex(int value){
    Node* temp = head;

    int idx = 0;

    while(temp != NULL){
        //cout<<temp->value <<" ";
        if(temp->value == value)
            return idx;
        temp = temp->next;
        idx++;
    }
    return -1; // not found
}

bool LinkedList::remove(int value){
    //cout<<endl;
    //cout<<"removing "<<value<<endl;
    Node* n = search(value);
    if(n == NULL)
        return false; // value not exist
    
    if(head == n){
        head = n->next;
    }

    Node* prevNode = n->prev;
    Node* nextNode = n->next;
    if(prevNode!=NULL)
        prevNode->next = nextNode;
    if(nextNode != NULL)
        nextNode->prev = prevNode;
    

    delete(n);
    length--;
    return true; // successfully removed
}

int LinkedList::size(){
    return this->length;
}


int LinkedList::print(){
    Node* temp = head;
    int lenght = 0;
    while(temp != NULL){
        cout<<temp->value <<" ";
        temp = temp->next;
        lenght++;
    }
    cout<<endl;
    return lenght;
}



// for testing
/* int main(){
    LinkedList list;
    //list.insertFront(30);
    list.insertFront(53);
    list.insertFront(4);
    list.insertFront(67);
    list.print();
    list.insertFront(52);
    list.print();

    cout<<"find index: "<<list.findIndex(53)<<endl;

    Node* s = list.search(67);
    if(s == NULL){
        cout<<"not found"<<endl;
    }else{
        cout<<"found "<<s->value<<endl;
    }

    cout<<(list.remove(4) ?"4 removed" : "not exist")<<endl;
    int l = list.print();
    cout<<"length: "<<l<<endl;

    cout<<(list.remove(52) ?"52 removed" : "not exist")<<endl;
    l = list.print();
    cout<<"length: "<<l<<endl;

    cout<<(list.remove(53) ?"53 removed" : "not exist")<<endl;
    l = list.print();
    cout<<"length: "<<l<<endl;

    cout<<(list.remove(67) ?"67 removed" : "not exist")<<endl;
    l = list.print();
    cout<<"length: "<<l<<endl;
} */