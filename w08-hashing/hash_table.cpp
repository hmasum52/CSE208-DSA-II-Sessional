#include<bits/stdc++.h>
#include "linked_list.cpp"
using namespace std;
typedef long long ll;

class HashTable{
    
    int n; 
    vector<LinkedList> tableSC; // table for separte chaining
    vector<int> tableP; // table for double and custom probing
    int func; // 1 for hash1(), 2 for hash2()
    int method;

    bool isPrime(int);
    int nextPrime(int);

    int insertBySeparateChaining(string key, int value);
    int insertByProbing(string key, int value);

    bool deleteBySeparateChaining(string key, int value);
    bool deleteByProbing(string key, int value);

    pair<int, int> searchBySeparateChaining(string key, int value);
    pair<int, int> searchByProbing(string key, int value);


    int hash1(string s);
    int hash2(string s);
    int hash(string s);
    int auxHash(string s);
    int doubleHash(string s, int itr);
    int customHash(string s, int itr);
    int methodHash(string s, int itr);

    public: // public function and variables
    const static int NIL = -1; // NULL value for probing table
    const static int DELETED = -2; // for delete for probing table
    const static int HASH1 = 1;
    const static int HASH2 = 2;
    const static int SEPARATE_CHAINING = 3;
    const static int DOUBLE_HASHING = 4;
    const static int CUSTOM_PROBING = 5;
    int collisionCnt;

    HashTable(int, int func, int method);

    int insert(string key, int value);
    bool remove(string key, int value);
    pair<int, int> search(string key, int value); // return table index position and probe value

    // for debugging
    void printSC(){
        int i= 0;
        for(LinkedList l : tableSC){
            cout<< (i++) <<"-> ";
            l.print();
        }
    }

    // for debugging
    void printMaxCollisionIdx(){
        int idx = -1;
        int mx = -1;
        int i=0;
        for(LinkedList l : tableSC){

            if(l.size()> mx){
                idx = i;
                mx = l.size();
            }
            i++;
        }
        cout<<"mx-> "<<idx<<" : "<<mx<<endl;
    }
};

// constructor
HashTable::HashTable(int n, int func, int method){
    this->n = nextPrime(n);
    //cout<<"table size: "<<this->n<<endl;
    this->func= func;
    this->method = method;
    tableSC.resize(this->n);
    tableP.assign(this->n, -1);

    collisionCnt = 0;
}

// reference: https://cp-algorithms.com/string/string-hashing.html
// polinomial hash function
int HashTable::hash1(string s){
    //cout<<"hash1"<<endl;
    int p = 31;
    long long p_pow = 1; 
    const int m = 1e9 + 9;

    long long hashValue = 0;
    for (char c : s) {
        hashValue = (hashValue + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hashValue%n;
}

// source: http://www.cse.yorku.ca/~oz/hash.html
int HashTable::hash2(string s){
    long long hashValue = 0;
    const int m = 1e9 + 9;
    for(int i = 0 ; i<7; i++){
        hashValue = ((long long)s[i] + (hashValue << 6) + (hashValue<<16) - hashValue)%m;
    }
    return hashValue%n;
}

int HashTable::hash(string key){
    //cout<<"hash1"<<endl;
    return (func == HASH1) ? hash1(key) : hash2(key);
}

// source: http://www.cse.yorku.ca/~oz/hash.html
int HashTable::auxHash(string key){
    long long hash = 5381;
    int c;

    for(char c: key)
        hash = (((hash << 5) + hash) + c)%(ll)(1e9+7); /* hash * 33 + c */

    return hash%n;
}

int HashTable::doubleHash(string key, int itr){
    return (hash(key) + itr*auxHash(key))%n;
}

int HashTable::customHash(string key, int itr){
    int c1 = 17;
    int c2 = 19;
    return (hash(key) + c1*itr*auxHash(key) + c2*itr*itr) %n;
}

int HashTable::methodHash(string key, int itr){
    if(method == DOUBLE_HASHING)
        return doubleHash(key, itr);
    else if(method == CUSTOM_PROBING)
        return customHash(key, itr);
    return hash(key);
}

// =============== DICTIONARY OPERATIONS =========================

int HashTable::insert(string key, int value){
    return method == SEPARATE_CHAINING ?
                    insertBySeparateChaining(key, value)
                    : insertByProbing(key, value);
}

bool HashTable::remove(string key, int value){
    return method == SEPARATE_CHAINING ?
                    deleteBySeparateChaining(key, value)
                    : deleteByProbing(key, value);
}

pair<int, int> HashTable::search(string key, int value){
    return method == SEPARATE_CHAINING ?
                    searchBySeparateChaining(key, value)
                    : searchByProbing(key, value);
}


// ================ private util methods ================================

// Function that returns true if n
// is prime else returns false
bool HashTable::isPrime(int n)
{
    // Corner cases
    if (n <= 1)  return false;
    if (n <= 3)  return true;
   
    // This is checked so that we can skip 
    // middle five numbers in below loop
    if (n%2 == 0 || n%3 == 0) return false;
   
    for (int i=5; i*i<=n; i=i+6)
        if (n%i == 0 || n%(i+2) == 0)
           return false;
   
    return true;
}

int HashTable::nextPrime(int n){
    // Base case
    if (n <= 1)
        return 2;
 
    int prime = n;
    bool found = false;
 
    // Loop continuously until isPrime returns
    // true for a number greater than n
    while (!found) {
        prime++;
 
        if (isPrime(prime))
            found = true;
    }
 
    return prime;
}

// =============== PRIVATE DICTIONARY OPERATIONS =========================

// return the key index value where the value is inserted
int HashTable::insertBySeparateChaining(string key, int value){
    int i = hash(key);
   // cout<<"sc: "<<key<<" "<<i<<endl;

    if(tableSC[i].size()>0)
        collisionCnt++;
    
    tableSC[i].insertFront(value); // O(1)

    return i;
}

int HashTable::insertByProbing(string key, int value){
    int i = 0; //
    while(i<n){
        int index = methodHash(key, i);
        if(i>0)
            collisionCnt++;
        if(tableP[index] == NIL){
            tableP[index] = value;
            return index; // return the position where inserted
        }else{
            i++; // next iteration
        }
    }
    cout<<"Hash table overflow"<<endl;
    return -1; // overflow
}

bool HashTable::deleteBySeparateChaining(string key, int value){
    int i = hash(key);
    return tableSC[i].remove(value); //O(table[i].length)
}

bool HashTable::deleteByProbing(string key, int value){
    pair<int, int> p = searchByProbing(key, value);
    if(p.first == -1)
        return false;
    tableP[p.first] = DELETED; //mark deleted
    return true;
}

pair<int, int> HashTable::searchBySeparateChaining(string key, int value){
    int i = hash(key);
    int idx = tableSC[i].findIndex(value); //O(table[i].length)
    if(idx == -1){
        return {-1, 0}; // not found
    }
    return {i, idx+1}; // key index, probe cnt
}

// return {index, probe_cnt} if found
// return {-1 , 0} if not found
pair<int, int> HashTable::searchByProbing(string key, int value){
    int i = 0; 
    while(i<n){
        int index = methodHash(key, i);
        if(tableP[index] == value)
            return {index, i+1}; 
        if(tableP[index] == NIL){ //value doesn't exist
            return {-1, 0}; // return the position where inserted
        }else{
            i++; // next iteration
        }
    }
    cout<<"Hash table overflow"<<endl;
    return {-1, 0}; // not found
}