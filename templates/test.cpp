#include<bits/stdc++.h>
#include "graph.h"


int main(int argc, char const *argv[]){
    Graph g(5);
    int n, m; //number of nodes
    cin>>n>>m;

    for (int i = 0; i < n; i++)
    {
        int u, v;
        cin>>u>>v;
        g.addUndirectedEdge(u, v);
    }

   // cout<<"graph input completed"<<endl;

    cout<<g.isBipartite()<<endl;
    
    return 0;
}
