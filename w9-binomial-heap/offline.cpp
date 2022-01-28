#include<bits/stdc++.h>
using namespace std;

class Node{
    public:
    int key; 
    int value; // application required info
    int degree; // number of children of the node
    Node* parent; // pointer to parent node. NULL if the node is parent 
    Node* leftChild; // left most child pointer if exist, NULL other wise
    Node* rightSibling; // the sibling pointer immediately to its right
};




int main(){
    //read from file
    freopen("in.txt", "r", stdin);

    
    return 0;
}