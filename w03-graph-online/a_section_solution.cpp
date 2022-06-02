#include <bits/stdc++.h>
using namespace std;

class Graph
{
    // private properties ==================================

    // store graph in adjaceny list
    int n; // number of nodes
    vector<vector<int>> adj;

    // track if a node is visited
    // while traversing the graph
    vector<int> visited;

    /**
     * @brief reset all vistied to 0
     * before traversing the graph
     * 
     */
    void initVisited()
    {
        visited.assign(n, 0);
    }

    // public properties ====================================
public:
    /**
     * @brief Construct a new Graph object
     * 
     * @param n is the number of nodes in the graph
     */
    Graph(int n)
    {
        this->n = n;
        adj.resize(n);
    }

    /**
     * @brief add and edge to a undirected graph
     * we assume nodes are from 0 to n-1
     * 
     * @param u starting node of the edge.
     * @param v ending node of the edge.
     */
    void addDirectedEdge(int u, int v)
    {
        adj[u].push_back(v);
    }

    /**
     * @brief this method return the source city 
     * from which all other cities are reachable
     * 
     * @return int the source city 
     * if not possible return -1
     */
    int getsourcecity(){

        initVisited();

        stack<int> dfs_order;
        for (int i = 0; i < n; i++)
        {
            if(!visited[i]){
                dfs(i, dfs_order);
            }
        }
        cout<<endl;

        int lastvisted = dfs_order.top();

        // cout<<"last visited "<<lastvisted<<endl;
        // it can be proved that last viisted node in dfs traversal
        // is the source from which we can reach all the nodes
        // if we can't reach any node from source there exist no path 
        // from that node to source
        return checkReachable(lastvisted) ? lastvisted : -1;
    }

    bool checkReachable(int source){
        initVisited();
        stack<int> reachableNodes;
        dfs(source, reachableNodes);
        return reachableNodes.size() == n; // all nodes are reachable from source
    }

    void dfs(int u, stack<int> &order){
        visited[u] = 1;
        for(int &v: adj[u]){
            if(!visited[v])
                dfs(v, order);
        }
        //cout<<u<<" ";
        order.push(u);
    }
};

int main(int argc, char const *argv[])
{
    int n; // number of city
    cin>> n;
    int m; // number of roas
    cin>>m;

    Graph g(n);
    int u, v;
    for (int i = 0; i < m; i++)
    {
        cin>>u>>v;
        g.addDirectedEdge(u, v);
    }

    cout<<endl;

    cout<<g.getsourcecity()<<endl;
    
    return 0;
}
