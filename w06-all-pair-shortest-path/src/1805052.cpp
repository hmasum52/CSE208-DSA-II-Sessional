#include <bits/stdc++.h>
using namespace std;
const int INF = INT_MAX;

class Graph
{
    vector<vector<int>> adj; // adjacency matrix
    int n;                   // number of vertex
public:
    // we assume the graph starts from vertex 0
    Graph(int n)
    {
        this->n = n;
        adj.resize(n);
        for (int i = 0; i < n; i++)
        {
            // init adjacency matrix with INF
            // INF means there is no edge between vertex i to j
            adj[i].assign(n, INF);
            adj[i][i] = 0;
        }
    }

    // add a directed edge
    void addEdge(int u, int v, int w)
    {
        adj[u][v] = w; // add the edge in directed graph
    }

    void floydWarshall()
    {
        vector<vector<int>> dist = adj; // 2d n*n matrix

        for (int k = 0; k < n; k++)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    if( dist[i][k] != INF && dist[k][j] != INF) // there exist edge from i to k and k to j
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
        cout << "Shortest distance matrix" << endl;

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (dist[i][j] == INF)
                    cout << "INF ";
                else
                    cout << dist[i][j] << " ";
            }
            cout << endl;
        }
    }
};
int main()
{
    int n;
    cin >> n;
    int m;
    cin >> m;

    Graph g(n);
    for (int i = 0; i < m; i++)
    {
        int u, v;
        double w;
        cin >> u >> v >> w;
        u--;
        v--;
        g.addEdge(u, v, w);
    }

    g.floydWarshall();

    return 0;
}