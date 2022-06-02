#include <bits/stdc++.h>
using namespace std;

class Edge{
public:
    int u;
    int v;
    double w;

    Edge(int u, int v, double w){
        this->u = u;
        this->v = v;
        this->w = w;
    }

    bool operator<(Edge const &other){
        // for a edge (from, to , weight)
        if( w != other.w) // 1st sort by weight
            return w < other.w;
        
        if (v != other.v) // then sort by to
            return v < other.v;

        return u < other.u; // then sort by from
    }
};

class Graph{
    // private properties ==================================

    // store graph in adjaceny list
    bool directed;
    int n;

    // for dijkstra
    vector<vector<pair<int, double> > > adj;
    // for bellman ford
    vector<Edge> edges;

public:
    static const int DIJKSTRA = 1;
    static const int BELLMANFORD = 2;

    /**
     * @brief Construct a new Graph object
     * 
     * @param n is the number of nodes in the graph
     * @param directed denotes if the graph is directed or undirected
     */
    Graph(int n, bool directed)
    {
        this->n = n;
        this->directed = directed;
        adj.resize(n);
    }

    /**
     * @brief add and edge to a undirected graph
     * we assume nodes are from 0 to n-1
     * 
     * @param u starting node of the edge.
     * @param v ending node of the edge.
     * @param w is the weight of each edge
     */
    void addEdge(int u, int v, double w){
        // for dijkstra
        adj[u].push_back({v, w});
        if(directed)
            adj[v].push_back({u, w});

        // for bellman ford
        edges.push_back(Edge(u, v, w));
    }

    void shortestPath(int from, int to){
        pair<vector<double>, vector<int> > distParent = dijkstra(from);
        vector<double> dist = distParent.first;
        vector<int> parent = distParent.second;
        if(dist[to] == INT_MAX){
            cout<<"Shortest path doesn't exist from "<<from <<" to "<<to<<endl;
            return;
        }
        cout<<"Shortest path cost: "<<dist[to]<<endl;
        printShortestPath(parent, to, to);
        cout<<endl;
    }

    void shortestPath(int from, int to, int algo){
        pair<vector<double>, vector<int> > distParent = algo == DIJKSTRA ? dijkstra(from) : bellmanFord(from);
        vector<double> dist = distParent.first;
        vector<int> parent = distParent.second;

        if(algo == BELLMANFORD){
            if(dist.size() == 0){
                cout<<"The graph contains a negative cycle"<<endl;
                return;
            }
            cout<<"The graph does not contain a negative cycle"<<endl;
        }
        
        cout<<"Shortest path cost: "<<dist[to]<<endl;
        printShortestPath(parent, to, to);
        cout<<endl;
    }

    void printShortestPath(vector<int> parent, int v, int to){
        if(parent[v]!=-1)
            printShortestPath(parent, parent[v], to);

        cout<<v<< (v != to ? " -> " : "\n");
    }

    /**
     * @brief 
     * 
     * @param source is the source of each shortest path
     * @return pair<vector<double>, vector<int> > {dist , parent}
     */
    pair<vector<double>, vector<int> > dijkstra(int source){
        vector<double> dist(n, INT_MAX); // distance of each vertex from source
        vector<int> parent(n, -1); // parent of each vertex in the shortest path
        vector<int> vis(n, false); // track if a vertex is in the priority queue
        
        dist[source] = 0; // distance of source is zero

        // min heap priority queue
        priority_queue<pair<double, int>, vector<pair<double,int> > , greater<pair<double, int> > > pq; // priority queue of {dist, vertex} pair
        pq.push({0, source});

        double cost = 0;

        // step 2: O(elogv)
        while(!pq.empty()){
            // extract min edge
            pair<int,int> p = pq.top(); // logn
            pq.pop();

            double d = p.first; // distance
            int u = p.second; // from vertex

            //cout<<"poped vertex "<<u<<" with key "<<d<<endl;
           
            if(vis[u]) // distance already minimized
                continue;

            vis[u] = true; // done visting 

            cost += d;
        
            for(pair<int, double>& vw: adj[u]){
                int v = vw.first; // to
                double w = vw.second; // weight

               // cout<<"edge ("<<u<<", "<<v<<", w = "<<w<<")"<<endl;

                if(vis[v]) // already added in mst
                    continue;

                if(dist[u] + w < dist[v]){
                    dist[v] = dist[u] + w; // update key with new min weight
                    parent[v] = u; // update parent of v
                    pq.push({dist[v], v}); // logn
                    // cout<<"updated key of vertex "<<v<<" to "<<dist[v]<<endl;
                }
               
            }
        }
        /* for(int i = 0 ; i<dist.size() ; i++)
            cout<<"( "<<i<<", "<<dist[i]<<")";
        cout<<endl; */
        
        return {dist, parent};
    }

    pair<vector<double>, vector<int> >  bellmanFord(int source){
        // step-1: init all the vertex distance with infinite vertex value
        vector<double> dist(n, INT_MAX);
        vector<int> parent(n, -1);

        // step-2: init source distance
        dist[source] = 0;

        // relaxation of each vertex total |v|-1 times
        for(int i = 0; i< n;i++){
            for(Edge e : edges){
                // relaxation of each edge
                if(dist[e.u] + e.w < dist[e.v]){
                     dist[e.v] = dist[e.u] + e.w;
                     parent[e.v] = e.u;
                }   
            }
        }

        // search of negative cycle
        for(Edge e : edges){
                // relaxation of each edge
                if(dist[e.u] + e.w < dist[e.v]){
                     //cout<<e.u<<" "<<e.v<<" "<<e.w<<endl;
                     return {};
                }
                   
        }

        return {dist, parent};
    }
};

int main()
{
    int n;
    cin >> n;
    int m;
    cin >> m;

    Graph g(n, false);
    for (int i = 0; i < m; i++)
    {
        int u, v;
        double w;
        cin >> u >> v >> w;
        g.addEdge(u, v, w);
    }

    int source, destination;
    cin>>source >> destination;

    g.shortestPath(source , destination, Graph::DIJKSTRA);
}