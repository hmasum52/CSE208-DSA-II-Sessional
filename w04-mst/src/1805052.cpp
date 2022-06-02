#include <bits/stdc++.h>
using namespace std;

class DisjointSetForest{
    /**
     * @brief track the representative of x
     */
    vector<int> parent;
    /**
     * @brief 
     * rank is the upper bound on the height of each node
     */
    vector<int> rank;

public:
    DisjointSetForest(int size)
    {
        parent.clear();
        rank.clear();
        parent.resize(size);
        rank.resize(size);
    }

    /**
     * @brief creates a new set whose only member is x
     * x is parent of itself and has a rank 0
     * 
     * @param x only member of the set.
     */
    void makeSet(int x)
    {
        parent[x] = x;
        rank[x] = 0;
    }

    /**
     * @brief find the representative of the set in which x belongs.
     * Uses path compression which makes x to directly points to its
     * representative.
     * 
     * @param x a member of a disjoint set
     * @return the representative of the set in which x belongs
     */
    int findSet(int x)
    {
        //cout<<"inside find set"<<endl;
        if (parent[x] != x)
            parent[x] = findSet(parent[x]);
        //cout<<"find set 3: "<<parent[x]<<endl;
        return parent[x];
    }

    /**
     * @brief a subroutine called by Union
     * which takes two representative of 2 set and link the two sets
     * 
     * @param x is the representative of 1st set
     * @param y is the representative of 2nd set
     */
    void link(int x, int y)
    {
        if (rank[x] > rank[y])
        {
            parent[y] = x;
        }
        else
        {
            parent[x] = y;
            if (rank[x] == rank[y]) // if rank match we update the rank of y
            {
                rank[y]++;
            }
        }
    }

    void Union(int x, int y)
    {
        // cout<<"union of "<<x<<" "<<y<<endl;
        link(findSet(x), findSet(y));
    }

    void debugPrintParent()
    {
        cout << "p: ";
        for (int &i : parent)
            cout << i << " ";
        cout << endl;
    }

    void debugPrintRank()
    {
        cout << "r: ";
        for (int &i : rank)
            cout << i << " ";
        cout << endl;
    }
};

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

 class PrimVertex{
        public:
        double key;
        int vertex;
        PrimVertex(int vertex, double key){
            this-> vertex = vertex;
            this-> key = key;
        }
    };

bool operator<(PrimVertex a, PrimVertex b){
    return a.key > b.key;
}

class Graph{
    // private properties ==================================

    // store graph in adjaceny list
    int n; // number of nodes
    vector<Edge> edges;
    vector<vector<pair<int, double> > > adj;
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
    void addEdge(int u, int v, double w){
        // for kruskal
        edges.push_back(Edge(u, v, w));

        // for Prim's
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void sortByEdgeWeight(){
        sort(edges.begin(), edges.end());
    }


    /**
     * @brief 
     * 
     * @return pair<double, vector<Edge>> cost and mst as a pair
     */
    pair<double, vector<Edge>> mstKruskal(){
        // cout << "inside kruskal" << endl;

        vector<Edge> a; // empty set
        DisjointSetForest dsu(n);
        // for each vertex in the graph do dsu makeset operation
        for (int v = 0; v < n; v++){
            dsu.makeSet(v);
        }
        //cout << "make set done" << endl;

        //dsu.debugPrintParent();
        //dsu.debugPrintRank();

        // sort the edges into non-decreasing order by weight
        sortByEdgeWeight(); // O(ElogE)
        //printDebug();
        //cout << "edge sorted successfully" << endl;

        // now for each edge (u,v) taken in non-decreasing order
        // do dsu findSet and union the set if findSet(u) != findSet(v)
        double cost = 0.0;
        for (Edge &e : edges){
            //cout << endl<< "processing edge (" << e.u << ", " << e.v << ") w = " << e.w << endl;

            // check if both start and end vertex are in the same set or not
            // if the vertices are not in the same set we can add it to mst
            // if the vertices are in the same set then adding edge will create a cycle
            if (dsu.findSet(e.u) != dsu.findSet(e.v)){
                a.push_back(e); // add the edge to mst
                cost += e.w;
                dsu.Union(e.u, e.v); // union to set
               // cout << "adding edge " << e.u << " " << e.v << " " << e.w << endl;
            }
            else{
                //cout << "cycle detected. Ignoring edge (" << e.u << ", " << e.v << ")" << endl;
            }

            //dsu.debugPrintParent();
            //dsu.debugPrintRank();
        }

        return {cost, a};
    }

    /**
     * @brief 
     * 
     * @return pair<double, vector<Edge>> cost and mst as a pair
     */
    pair<double, vector<Edge>> mstPrims(){
        vector<double> key(n); // key property of each vertex
        vector<int> parent(n); // parent of each vertex in mst 
        vector<Edge> mst; 
        vector<int> is_in_pq(n, true); // track if a vertex is in the priority queue

        // step 1: O(V)
        for(int u = 0; u<n;u++){
            key[u] = INT_MAX; // infinity
            parent[u] = -1; // NILL
        }

        
        int root = 0;
        key[root] = 0; // start Prim's from vertex 0
        priority_queue<PrimVertex> pq; // key, vertex
        // step 2: O(vlogv)
        for (int i = 0; i < n; i++){
            pq.push(PrimVertex(i, key[i]));
        }

        double cost = 0;

        // step 2: O(elogv)
        while(!pq.empty()){
            // extract min edge
            PrimVertex p = pq.top(); // logn
            pq.pop();
            int u = p.vertex; // from
           
            if(!is_in_pq[u]) // already added in mst
                continue;

            cost += p.key;
            //cout<<u<<" ";
            if(parent[u] != -1)
                mst.push_back(Edge(parent[u] ,u , p.key));

            is_in_pq[u] = false; // added to mst
        
            for(pair<int, double>& vw: adj[u]){
                int v = vw.first; // to
                double w = vw.second; // weight

                if(!is_in_pq[v]) // already added in mst
                    continue;

                if(w < key[v]){
                    key[v] = w; // update key with new min weight
                    parent[v] = u; // update parent of v
                    pq.push(PrimVertex(v, w)); // logn
                }
            }
        }
        /* cout<<endl;
        cout<<"Prims cost: "<<cost<<endl;
        for(Edge& e: mst){
            cout << "(" << e.u << "," << e.v << "),";
        } */
        return {cost, mst};
    }

    void printDebug(){
        for (Edge &e : edges){
            cout << e.u << " " << e.v << " " << e.w << endl;
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
        g.addEdge(u, v, w);
    }

    pair<double, vector<Edge>> prims = g.mstPrims();
    cout << "Cost of minimum spanning tree: " << prims.first << endl;

    cout << "List of edges selected by Prim's: {";
    for(int i = 0; i<prims.second.size(); i++){
        Edge e = prims.second[i];
        cout << "(" << e.u << "," << e.v;
        if(i!=prims.second.size()-1)    
            cout << "),";
        else{
            cout<<")}";
        }
    }
    cout<<endl;

    pair<double, vector<Edge>> kruskal = g.mstKruskal();
    //cout << "Cost of minimum spanning tree: " << kruskal.first << endl;

    cout << "List of edges selected by Kruskal's: {";
    for(int i = 0; i<kruskal.second.size(); i++){
        Edge e = kruskal.second[i];
        cout << "(" << e.u << "," << e.v;
        if(i!=kruskal.second.size()-1)
            cout << "),";
        else{
            cout<<")}";
        }
    }
    cout<<endl;
}