#include<bits/stdc++.h>
using namespace std;


class BinomialTree{
    public:
    int key; 
    int value; // application required info
    int degree; // number of children of the node
    BinomialTree* parent; // pointer to parent node. NULL if the node is parent 
    BinomialTree* child; // left most child pointer if exist, NULL other wise
    BinomialTree* sibling;

    BinomialTree(){

    }
    BinomialTree(const BinomialTree& t){
        key = t.key;
        value = t.value;
        degree = t.degree;
        child = t.child;
        sibling = t.sibling;
        parent = t.parent;
    }
    BinomialTree(int key, int value){
        this->key = key;
        this->value = value;
        this->degree = 0;
        parent = NULL;
        child = NULL;
        sibling = NULL;
    }
     BinomialTree(int key){
        this->key = key;
        this->value = 0;
        this->degree = 0;
        parent = NULL;
        child = NULL;
        sibling = NULL;
    }
};

class BinomialHeap{
    public:
    BinomialTree* head;

    BinomialHeap(){
        head = NULL;
    }

    BinomialHeap(int root){
        head = new BinomialTree(root);
    }

    BinomialHeap(const BinomialHeap &h){
        head = h.head;
    }

    BinomialTree* findMax(){
        BinomialTree* temp = head;

        int mx = INT_MIN;
        BinomialTree* mxTree = temp;
        while(temp != NULL){
            if(temp->key > mx){
                mx = temp->key;
                mxTree = temp;
            }
            temp = temp->sibling;
        }
        return mxTree;
    }


    pair<BinomialTree*, BinomialTree*> extractMax(){
        //cout<<"Inside ex max"<<endl;
        BinomialTree* temp = head;
        BinomialTree* prev = NULL;

        int mx = INT_MIN;
        BinomialTree* mxTree = temp;
        while(temp != NULL){
            if(temp->key > mx){
                mx = temp->key;
                prev = mxTree;
                mxTree = temp;
            }
            temp = temp->sibling;
        }

        if(mxTree == NULL)
            return {NULL, NULL};

        //cout<<"mx: "<<mxTree->key<<" prev: "<<prev->key<<endl;
        if(prev!=NULL)
            prev->sibling = mxTree->sibling;
        
        BinomialTree* child = mxTree->child;
        child->parent = NULL;
        BinomialTree* rightChild = child->sibling;
        child->sibling = NULL;

        //cout<<"child of "<<mxTree->key<<": ";
        while( rightChild != NULL){
            //cout<<mxTree<<" "<<child<<" "<<rightChild<<endl;
            //cout<<mxTree->key<<" "<<child->key<<" "<<rightChild->key<<" ";
            rightChild->parent = NULL;
            BinomialTree* temp = rightChild->sibling;
            rightChild->sibling = child;
            child = rightChild;
            rightChild = temp;
        }
        //cout<<endl;
        return {mxTree, child};
    }

   
    void increaseKey(int prevKey, int newKey){
        BinomialTree* tree = find(head, prevKey);

        if(tree == NULL){
            //cout<<"key "<<prevKey<<" not found "<<endl;
            return;
        }

        tree->key = newKey; // update key value

        BinomialTree* parent = tree->parent;
        while(parent != NULL){
           // cout<<"parent : "<<parent<<" "<<parent->key<<endl;
           // cout<<"child: "<<tree<<" "<<tree->key<<endl;
            if(parent->key < tree->key){ // violates the heap property 
                // swap value
                int temp = parent->key;
                parent->key = tree->key;
                tree->key = temp;

                tree = parent;
                parent = tree->parent;
               // if(parent == NULL)
                    //cout<<"ops parent null"<<endl;
            }else{
                return;
            }
        }

    }


    BinomialTree* find(int key){
        return find(head, key);
    }


    BinomialTree* find(BinomialTree *root, int key){
        //cout<<"BinomialHeap::find root: "<<root->key<<" "<<root<<" p: "<<root->parent<<" c: "<<root->child<<endl;
        BinomialTree* temp = root;

        BinomialTree* f = NULL;
        if(temp != NULL){
            if(temp->key == key){
                //cout<<"found "<<temp->key<<" "<<root->key<<endl;
                return temp;
            }
            if(temp->sibling != NULL){
                f = find(temp->sibling , key);
                if(f!=NULL)
                    return f;
            }
                
            if(temp->child != NULL){
                 f = find(temp->child, key);
            }
               
        }
        return f;
    }


    /* BinomialHeap(BinomialTree* root){
        this->head = root;
    } */
    void print(){
        //cout<<"inside non recursive print"<<endl;
        BinomialTree* bi = head;
        while(bi != NULL){
            cout<<"Binomial Tree, B"<<(bi->degree)<<endl;
            
            cout<<"Level 0 : "<<bi->key<<endl;
            if(bi->child != NULL){
                cout<<"Level 1 : ";
                BinomialTree* t = bi->child;
                while(t != NULL){
                    cout<<t->key<<" ";
                    t = t->sibling;
                }
                cout<<endl;
                if(bi->child->child!=NULL)
                    print(bi->child->child, 2);
            }
            bi = bi->sibling;
            //cout<<endl;
        }//cout<<"done printing"<<endl;
    }
    void print(BinomialTree * root, int levelCnt){
        //cout<<"inside recursive print"<<endl;
        BinomialTree* level = root;
        cout<<"Level "<<levelCnt<<" : ";
        while(level != NULL){
            BinomialTree* temp = level;
            while(temp!=NULL){
                cout<<temp->key<<" ";
                temp = temp->sibling;
            }
            level = level->parent->sibling;
            if(level!=NULL)
                level = level->child;
        }

        if(root->child!=NULL){
             print(root->child, ++levelCnt);
        }
    }
};

BinomialTree* copyTree(BinomialTree* t){
    //cout<<"copy "<<t->key<<endl;
    BinomialTree* nt = new BinomialTree(t->key);
    nt->value = t->value;
    nt->parent = t->parent;
    nt->sibling = t->sibling;
    nt->child = t->child;
    nt->degree = t->degree;
    return nt;
}

BinomialTree* unionBinomialTree(BinomialTree* t1, BinomialTree* t2){
    //cout<<"inside unionBinomialTree"<<endl;
    //cout<<"t1: "<<t1<<" "<<t1->key<<endl;
    //cout<<"t2: "<<t2<<" "<<t2->key<<endl;
    BinomialTree* copyT1 = copyTree(t1);
    
    BinomialTree* copyT2 = copyTree(t2);
    //cout<<"ok"<<endl;
    //cout<<"t1: "<<t1<<" copyT1: "<<copyT1<<" "<<t1->key<<endl;
   // cout<<"t2: "<<t2<<" copyT2: "<<copyT2<<" "<<t2->key<<endl;
    //cout<<"copy done"<<endl;
    if(t1->child != NULL)
        t1->child->parent = copyT1;
    if(t2->child != NULL)
        t2->child->parent = copyT2;

    

    if(copyT1->key > copyT2->key){
        copyT2->parent = copyT1; // t1 becomes parent of t2
        copyT2->sibling = copyT1->child; // t2 becomes a sibling of the left most child of t1
        copyT1->degree++; // degree of t1 increased by 1
        copyT1->child = copyT2;
    
        return copyT1;
    }else{
        copyT1->parent = copyT2; // t2 becomes parent of t1
        copyT1->sibling = copyT2->child; // t1 becomes a sibling of the left most child of t2
        copyT2->degree++;
        copyT2->child = copyT1;
      
        return copyT2;
    }
}


BinomialHeap unionHeapTree(BinomialTree*t1, BinomialTree* t2){
   // cout<<"inside union heap"<<endl;
    BinomialHeap h;
    h.head = new BinomialTree(-1);

    BinomialTree* temp = h.head;

    BinomialTree *carry = NULL;

    while (t1 !=NULL && t2 !=NULL){
       // cout<<"ok22"<<endl;
        if(t1->degree > t2->degree){ 
            if(carry != NULL){ // 1(carry) + 0 + 1
              //  cout<<"1(carry) + 0 + 1"<<endl;
                carry = unionBinomialTree(carry, t2);
                t2 = t2->sibling;
                carry->sibling = NULL;
            }else{  // 0 + 1
               // cout<<"0(carry) + 0 + 1"<<endl;
                temp->sibling = t2; //
                temp = t2;
                t2 = t2->sibling;
            }
           
        }else if(t2->degree > t1->degree){ // 1 + 0
            if(carry != NULL){ // 1(carry) + 1 + 0
              //  cout<<"1(carry) + 1 + 0"<<endl;
                carry = unionBinomialTree(carry, t1);
                t1 = t1->sibling;
                carry->sibling = NULL;
            }else{  // 1 + 0
               // cout<<"0(carry) + 1 + 0"<<endl;
                temp->sibling = t1; //
                temp = t1;
                t1 = t1->sibling;
            }
           
        }else{
            if(carry != NULL) // 1(carry) + 1 + 1
            {
               // cout<<"1(carry) + 1 + 1"<<endl;
                carry = unionBinomialTree(carry, t1);
                temp = temp->sibling;
                temp = t2;
            }else{ // // 0(carry) + 1 + 1
               // cout<<"0(carry) + 1 + 1"<<endl;
               // cout<<"t1: "<<t1->key<<" , t2: "<<t2->key<<endl;
                carry = unionBinomialTree(t1, t2);
              
            }
            t1 = t1->sibling;
            t2 = t2->sibling;
            carry->sibling = NULL;
        }   
    }

    while(t1 != NULL){
       // cout<<"t1 is still not done"<<endl;
        if(carry != NULL && carry->degree == t1->degree){ // 1(carry) + 1 + 0
           // cout<<"1(carry) + t1 + 0"<<endl;
           // cout<<"carry: "<<carry->key<<" , t1: "<<t1->key<<endl;
            carry = unionBinomialTree(carry, t1);
            t1 = t1->sibling;
            carry->sibling = NULL;
            //break;
        }else if(carry != NULL){
          //  cout<<"1(carry) + 0 + 0"<<endl;
            temp->sibling = carry;
            temp = carry;
            carry = NULL;
        }
        else{  // 1 + 0
           // cout<<"0(carry) + t1 + 0"<<endl;
            temp->sibling = t1; //
            temp = t1;
            t1 = t1->sibling;
        }
        
    }

    while(t2 != NULL){
       // cout<<"t2 is not null value: "<<t2->key<<endl;

        if(carry != NULL && carry->degree == t2->degree){ // 1(carry) + 1 + 0
          //  cout<<"1(carry) + 0 + t2"<<endl;
          //  cout<<"carry: "<<carry->key<<" , t2: "<<t2->key<<endl;
            carry = unionBinomialTree(carry, t2);
            t2 = t2->sibling;
            carry->sibling = NULL;
            //break;
        }else if(carry != NULL){
           // cout<<"1(carry) + 0 + 0"<<endl;
            temp->sibling = carry;
            temp = carry;
            carry = NULL;
        }
        else{  // 1 + 0
           // cout<<"0(carry) + 0 + t2"<<endl;
            temp->sibling = t2; //
            temp = t2;
            t2 = t2->sibling;
        }
    }

    if(carry != NULL){
       // cout<<"1(carry) + 0 + 0"<<endl;
        temp->sibling = carry;
    }else{
       // cout<<"final carry is null"<<endl;
    }

    
    temp = h.head;
    h.head = h.head->sibling;
    delete temp;

    /* BinomialTree* kop = h.head;
    while(kop != NULL){
        cout<< kop<<" "<<kop->key<<endl;
        kop = kop->sibling;
    }
    
    h.print(); */
    return h;
}

BinomialHeap unionHeap(BinomialHeap h1, BinomialHeap h2){
    return unionHeapTree(h1.head, h2.head);
}

class PrioriytQueue{
    public:
    BinomialHeap h;

    PrioriytQueue(){
        
    }

    int findMax(){
        BinomialTree* mx = h.findMax();
        return  mx !=NULL ? mx->key : INT_MIN;
    }

    void insert(int value){
        //cout<<"PrioriytQueue::inserting "<<value<<endl;
        BinomialHeap h1(value);
        h = unionHeap(h, h1);
    }

    void print(){
        cout<<"Printing Binomial Heap..."<<endl;
        cout<<"--------------------------"<<endl;
        h.print();
        cout<<endl;
    }

    bool find(int value){
        cout<<"PrioriytQueue::finding "<<value<<endl;
        BinomialTree* tree = h.find(value);
        if(tree != NULL){
            cout<<tree->key<<" found"<<endl;
            return true;
        }else{
            cout<<value<<" not found"<<endl;
            return false;
        }
    }

    void increaseKey(int prevKey, int newKey){
        //cout<<"PrioriytQueue::increasing key "<<prevKey<<" to "<<newKey<<endl;
        h.increaseKey(prevKey, newKey);
        //cout<<"PrioriytQueue::icreased key "<<prevKey<<" to "<<newKey<<endl;
    }

    int exractMax(){
       // cout<<"PrioriytQueue::exracting max ";
        pair<BinomialTree*, BinomialTree*> p =  h.extractMax();
       // cout<<"mx: "<<p.first->key<<" child: "<<p.second->key<<endl;
       /*  BinomialTree* temp = p.second;
        while(temp != NULL){
            cout<<temp->key<<" ";
            temp = temp->sibling;
        } */
        //cout<<endl;
        h = unionHeapTree(h.head, p.second);
        return p.first != NULL ? p.first->key : INT_MIN;
    }
};

int main(){
    //read from file
    freopen("in.txt", "r", stdin);
    PrioriytQueue pq;

   /*  pq.insert(7);
    cout<<endl;
    pq.insert(12);
    cout<<endl;
    pq.insert(19);
    cout<<endl;
    pq.insert(5);
    cout<<endl;
    pq.insert(16);
    cout<<endl;
    pq.insert(6);
    cout<<endl;

    pq.print(); */
    string s;
    while(cin>>s){
        if(s=="INS"){
            int n; cin>>n;
            pq.insert(n);
            cout<<"Inserted "<<n<<endl;
        }else if(s=="PRI"){
            pq.print();
        }else if(s == "INC"){
            int key, newKey; cin>>key>>newKey;
            pq.increaseKey(key, newKey);
            cout<<"Increased "<<key<<". The updated value is "<<newKey<<"."<<endl;
        }else if(s == "FIN"){
            cout<<"FindMax returned "<<pq.findMax()<<endl;
        }else if( s== "EXT"){
             int mx =pq.exractMax();
             cout<<"ExtractMax returned "<<mx<<endl;
        }
    }
   
    //pq.find(66);
    
    return 0;
}