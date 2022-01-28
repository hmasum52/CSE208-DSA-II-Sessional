#include<bits/stdc++.h>
using namespace std;

vector<string> generateString(int n){
    srand(clock());

   // map<string, int> mp;
    vector<string> v;
    //int duplicateCnt = 0;
    for(int i = 0 ; i<n;i++){
        string s(7, '*');
        for(int j = 0; j<7; j++){
            int r = rand()%26;
            char ch = 'a'+r;
            s[j] = ch;
        }
        v.push_back(s);
        /* if(!mp[s]){
            mp[s] = 1;
            v.push_back(s);
        } 
        else{
            duplicateCnt++;
            n++;
            cout<<s<<endl;
        }  */
    }

   // cout<<"duplicate string cnt: "<<duplicateCnt<<endl;
    return v;
}


class Hash{
    int n;

    vector<vector<int> > separteChaining;
    public:

    Hash(){

    }

    Hash(int n){
        this->n = n;
        separteChaining.resize(n);
    }

    void insert(string key, int value){
        separteChaining[hash1(key, n)].push_back(value);
    }

    void search(string key){
       // separteChaining[]
    }

    void remove(string key, int value){
       // separteChaining[hash1(key, n)].er;
    }

    int hash1(string s, int m){
        vector<int> p  = {2, 3, 5, 7, 11, 13, 17};

        int hashValue = 0;
        for(int i = 0 ; i<7; i++){
            //cout<<s[i]<<" "<<(int)s[i]<<" "<<s[i]-'a'<<endl;
            hashValue = (hashValue + ((int)(s[i]-'a')+1)*(int)pow(p[i], i))%m;
        }
        return hashValue;
    }

};

int main(){

    int n = 30;
    cin>>n;
    vector<string> v =  generateString(n);
    cout<<"total string: "<<v.size()<<endl;
    Hash h(n);
    for(string s: v){
        cout<<s<<endl<<h.hash1(s, n)<<endl;
    }
    return 0;
}