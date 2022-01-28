#include<bits/stdc++.h>
#include "hash_table.cpp"
using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int random(int, int);

vector<string> generateString(int n){
    //srand(clock());

    map<string, int> mp;
    vector<string> v;
    int duplicateCnt = 0;
    for(int i = 0 ; i<n;i++){
        string s(7, '*');
        for(int j = 0; j<7; j++){
            int r = random(0, 25);//rand()%26;
            char ch = 'a'+r;
            s[j] = ch;
        }
        //v.push_back(s);
        if(!mp[s]){
            mp[s] = 1;
            v.push_back(s);
        } 
        else{
            duplicateCnt++;
            n++;
            cout<<s<<endl;
        } 
    }

   // cout<<"duplicate string cnt: "<<duplicateCnt<<endl;
    return v;
}

// https://codeforces.com/blog/entry/61587
int random(int start, int end){
    return uniform_int_distribution<int>(start, end)(rng);
}

void printResult(const string& name, HashTable& h, vector<pair<string, int>>& vv){
    cout<<name<<endl;
    cout<<"Collisions: "<<h.collisionCnt<<endl;
    int probe = 0;
    for(int i = 0; i<1000; i++){
        int r = random(0, vv.size()-1);
        probe += h.search(vv[r].first, vv[r].second).second;
    }
    cout<<"Avg Probes: "<<probe/1000.0<<endl<<endl;
}


int main(){
    int n = 10000;
    //cin>>n;
    vector<string> v =  generateString(n);
    vector<pair<string, int> > vv;

    cout<<"total string: "<<v.size()<<endl;
    //n = n*10;
    HashTable h1(n, HashTable::HASH1, HashTable::SEPARATE_CHAINING);
    HashTable h2(n, HashTable::HASH2, HashTable::SEPARATE_CHAINING);

    HashTable h3(n, HashTable::HASH1, HashTable::DOUBLE_HASHING);
    HashTable h4(n, HashTable::HASH2, HashTable::DOUBLE_HASHING);

    HashTable h5(n, HashTable::HASH1, HashTable::CUSTOM_PROBING);
    HashTable h6(n, HashTable::HASH2, HashTable::CUSTOM_PROBING);

    int i = 1;
    for(string s: v){
        h1.insert(s, i);
        h2.insert(s, i);
        h3.insert(s, i);
        h4.insert(s, i);
        h5.insert(s, i);
        h6.insert(s, i);
        vv.push_back({s, i++});
    }

    printResult("Chaining method(hash1)", h1, vv);
    printResult("Chaining method(hash2)", h2, vv);
   
    printResult("Double Hashing(hash1)", h3, vv);
    printResult("Double Hashing(hash2)", h4, vv);

    printResult("Custom Probing(hash1)", h5, vv);
    printResult("Custom Probing(hash2)", h6, vv);
   // h.printSC();
}