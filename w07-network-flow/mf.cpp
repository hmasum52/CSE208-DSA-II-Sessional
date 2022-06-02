#include<bits/stdc++.h>
using namespace std;
const int INF = INT_MAX;

class MaxFlowEdge{
    int u; // from
    int v; // to
    double c; // capcity
    double f; // flow

    public:

    MaxFlowEdge(){

    }

    MaxFlowEdge(int u, int v, double c){
        this-> u = u;
        this-> v = v;
        this-> c = c;
        this->f = 0;
       // cout<<"new edge: "<<this->u<<" , "<<this->v<<", "<<this->c<<endl;
    }

    MaxFlowEdge(const MaxFlowEdge &e){
        this->u = e.u;
        this->v = e.v;
        this->c = e.c;
        this->f = e.f;
        //cout<<"cc edge: "<<this->u<<" , "<<this->v<<", "<<this->c<<endl;
    }

    // getters
    int getFrom(){
        cout<<"from: "<<this->u<<endl;
         return u;
    }
    int getTo(){ return v;}
    double getCapacity(){return c;}
    double getFlow(){return f;}

    int otherEnd(int from){
        if(from == u)
            return v;
        else if(from == v)
            return u;
        return -1; // vertex doesn't belong to the edge.
    }

    double residualCapacity(int from, int to){
        //cout<<"u: "<<u<<" from: "<<from<<" v: "<<v<<" to: "<<to<<endl;
        if(from == u && to == v) // reverse
            return c - f;

        if(from == v && to == u) // forward
            return f;
        
        return -1; // no such edge
    }

    void addFlow(int from, int to, double flow){
       // cout<<u<<"->"<<v<<" f: "<<flow<<endl;
        if(from == u && to == v) // forward
        {
            f += flow;
            //cout<<"flow increased to "<<f<<endl;
        }  
        else if(from == v && to == u) // reverse
        {
             f-= flow;
            // cout<<"flow increased to "<<f<<endl;
        }   
        //else 
           // cout<<"None"<<endl;
    }
};

class MaxFlowNetwork{
    vector<vector<MaxFlowEdge>> adjMat;
    vector<vector<MaxFlowEdge>> adjList;
    public:

    /**
     * @brief Construct a new Max Flow Network object
     * 
     * @param n is the number of vertex
     */
    MaxFlowNetwork(int n){
        adjList.resize(n);
        adjMat.resize(n);
        for (int i = 0; i < n; i++){
            // init adjacency matrix with INF
            // INF means there is no edge between vertex i to j
            adjMat[i].resize(n);
        }
    }

    void addEdge(int u, int v, double capacity){
        MaxFlowEdge e(u, v, capacity);
         //cout<<"Ok "<<u<<" "<<v<<" "<<capacity<<endl;

        // add edge in both direction
        // for residual graph
        adjMat[u][v] = e;
       
        adjMat[v][u] = e;
       
        adjList[u].push_back(e);
        adjList[u].push_back(e);
    }

    vector<vector<MaxFlowEdge>>& adjacencyList(){
        return adjList;
    }

    void addFlow(int u, int v, double flow){
       // cout<<"inside add flow"<<endl;
        adjMat[u][v].addFlow(u, v, flow);
    }

    double residualCapacity(int from, int to){
        return adjMat[from][to].residualCapacity(from, to);
    }

    int size(){
        return adjMat.size();
    }
};

class EdmonKarp{
    vector<bool> visited;
    vector<MaxFlowEdge> edgeTo;
    double value;
    public:

    EdmonKarp(MaxFlowNetwork &g, int s, int t){
        value = 0.0;
        cout<<"edmon karp form "<<s<<" to "<<t<<endl;
        while(hasAgmentedPath(g, s, t)){
            double flow = minCapacityOfAugmentedPath(g, s, t);
            cout<<"capacity: "<<flow<<endl;
            addFlowToAugmentedPath(g, s, t, flow);
            value += flow;
        }
    }

    /**
     * @brief this method is called when there exist a path from s to t
     * 
     * @param s is the srouce vertex
     * @param t is the target vertex
     * @return double 
     */
    double minCapacityOfAugmentedPath(MaxFlowNetwork &g, int s, int t){
        // cout<<"inside bottleCapacityOfAugmentedPath"<<endl;
        double bottleCapacity = INF;
        cout<<"reverse path: "<<endl;
        //cout<<edgeTo[t].otherEnd(t)<<" "<<t<<"->"<<edgeTo[t].residualCapacity(edgeTo[t].otherEnd(t), t)<<endl;
        cout<<t<<"->";
        for(int v = t; v !=s ; v = edgeTo[v].otherEnd(v)){
            int u = edgeTo[v].otherEnd(v); 
            //cout<<u<<"-"<<v<<"->"<<g.residualCapacity(u, v)<<endl;
            cout<<u<<"->";
            bottleCapacity = min(bottleCapacity, g.residualCapacity(u, v));
        }
        //cout<<endl;
        return bottleCapacity;
    }

    void addFlowToAugmentedPath(MaxFlowNetwork &g, int s, int t, double flow){
        for(int v = t; v !=s ; v = edgeTo[v].otherEnd(v)){
            int u = edgeTo[v].otherEnd(v); 
            g.addFlow(u, v, flow);
        }
    }

    /**
     * @brief use bfs to find the agmented path if exist
     * augmented path is saved in edgeTo vector if found
     * 
     * @param g 
     * @param s 
     * @param t 
     * @return true is augmented path is found
     * @return false other
     */
    bool hasAgmentedPath(MaxFlowNetwork &g, int s, int t){
        edgeTo.resize(g.size());
        visited.assign(g.size(),false);

        queue<int> q;
        q.push(s);
        visited[s] = true;
       
       // cout<<s<<"->";
       
        while(!q.empty()){
            int u = q.front();
            //cout<<"ok4"<<endl;
            q.pop();
            vector<MaxFlowEdge>& gg = g.adjacencyList()[u];
            for(MaxFlowEdge &e : gg){
              // cout<<"ok5"<<endl;
              // cout<<e.getFrom()<<" to "<<e.getTo()<<endl;
                int v = e.otherEnd(u);
                //cout<<u<<", "<<v<<endl;
               // cout<<u<<",kop "<<v<<"-> "<<g.residualCapacity(u, v)<<endl;
                if( !visited[v] && g.residualCapacity(u, v) > 0){
                    //cout<<"ok6"<<endl;
                    //cout<<u<<", "<<v<<"->"<<endl;
                    //<<"visited "<<v<<endl;
                    edgeTo[v] = e;
                    visited[v] = true;
                    q.push(v);
                }
            }
        }

        //cout<<endl;

        return visited[t];
    }

    //getters
    double maxFlow(){
        return value;
    }

    bool inCut(int v){
        return visited[v]; // is v reachable from s in residual network
    }
    
};

int main(){
    int n, e; // number of vertex;
    int s, t; // source and target
    int u, v; // edge
    double c; // capacity

    cin>> n>>e;
    
    //cout<<"n = "<<n<<", e= "<<e<<endl;
    MaxFlowNetwork network(n);

    for (int i = 0; i < e; i++)
    {
        cin>>u>>v>>c;
        //cout<<u<<", "<<v<<", "<<c<<endl;
        network.addEdge(u, v, c);
    }

    cin>>s>>t;

    EdmonKarp edk(network, s, t);

    cout<<edk.maxFlow()<<endl;
    
}

