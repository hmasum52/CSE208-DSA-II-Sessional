#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp> 
#include <ext/pb_ds/detail/standard_policies.hpp>
using namespace __gnu_pbds;
using namespace std;
#define getbit(n, i) (((n) & (1LL << (i))) != 0) 
#define setbit0(n, i) ((n) & (~(1LL << (i)))) 
#define setbit1(n, i) ((n) | (1LL << (i))) 
#define togglebit(n, i) ((n) ^ (1LL << (i))) 
#define lastone(n) ((n) & (-(n))) 
char gap = 32;
#define int long long
#define ll long long 
#define lll __int128_t
#define pb push_back
typedef tree<
int,
null_type,
less<int>,
rb_tree_tag,
tree_order_statistics_node_update>
ordered_set;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
ll hashPrime = 1610612741;
#define N 200005

vector<int>adj[N];
stack<int>st;
int vis[N];

void dfs(int root) {
    vis[root] = 1;
    
    for(auto x : adj[root]) {
        if(vis[x] == 2) {
            continue;
        }
        else if(vis[x] == 1) {
            cout << "not possible\n";
            exit(0);
        }
        else {
            dfs(x);
        }
    }

    st.push(root);
    vis[root] = 2;
}

signed main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n;
    cin >> n;
    int d;
    cin >> d;
    
    for(int i = 1 ; i <= d ; i++) {
        int u, v;
        cin >> u >> v;
        adj[v].push_back(u);
    }

    for(int i = 1 ; i <= n ; i++) {
        if(vis[i] == 0) dfs(i);
    }
    
    while(!st.empty()) {
        cout << st.top();
        cout << " ";
        st.pop();
    }
    cout << "\n";
    return 0;
}