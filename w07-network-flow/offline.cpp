
#include<bits/stdc++.h>
using namespace std;
const int INF = 1e8+4;

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
    int getFrom(){return u;}
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

    bool isSaturated(){
        return c == f;
    }

    double residualCapacity(int from, int to){
        //cout<<"u: "<<u<<" from: "<<from<<" v: "<<v<<" to: "<<to<<endl;
        if(from == u && to == v) // forward capacity
            return c - f;

        if(from == v && to == u) // reverse capacity
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

    bool saturated(int u, int v){
        //cout<<"is saturated? "<<u<<" "<<v<<" "<<adjMat[u][v].isSaturated()<<endl;
        return adjMat[u][v].isSaturated();
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
        // cout<<"edmon karp form "<<s<<" to "<<t<<endl;
        while(hasAgmentedPath(g, s, t)){
            double flow = minCapacityOfAugmentedPath(g, s, t);
           // cout<<"capacity: "<<flow<<endl;
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
        // cout<<"reverse path: "<<endl;
        // cout<<edgeTo[t].otherEnd(t)<<" "<<t<<"->"<<edgeTo[t].residualCapacity(edgeTo[t].otherEnd(t), t)<<endl;
        //cout<<t<<"->";
        for(int v = t; v !=s ; v = edgeTo[v].otherEnd(v)){
            int u = edgeTo[v].otherEnd(v); 
           // cout<<u<<"-"<<v<<"->"<<g.residualCapacity(u, v)<<endl;
            //cout<<u<<"->";
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
                   // cout<<u<<", "<<v<<"->"<<g.residualCapacity(u, v)<<endl;
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
  
};

class Team{
    public:

    int id;
    string name;
    int w; // number of win
    int l; // number of losses
    int r; // number of remaining

};

int main(){
    int n; // number of teams
    cin>>n;
    vector<Team> teams;

    vector<vector<int>> matches(n+1);
    for(int i = 0;i<n;i++)
        matches[i].assign(n+1, 0);

    for(int i = 0; i<n; i++){
        Team t;

        t.id = i;
        cin>>t.name>>t.w>>t.l>>t.r;

        for(int j = 0 ;j<n;j++){
            cin>>matches[i][j];
        }

        teams.push_back(t);
    }


    for(int i = 0; i< n; i++){
        //cout<<i<<endl;
        int numberOfMatches = ((n-1)*(n-2))/2;
        int v = 2 + numberOfMatches + n;
        MaxFlowNetwork network(v);

        int source = n; // srouce vertex
        int target = n+1; // target vertex

        int temp = n+2;

        for(int j = 0 ; j<n;j++){
            for(int k = j+1 ; k<n;k++){

                if( i == j || i == k) // ignore i's game
                    continue;

                network.addEdge(source, temp, matches[j][k]);
                //cout<<"added edge "<<source<<" to "<<temp<<" = "<<matches[j][k]<<" between "<<j <<" and "<<k<<endl;
               // cout<<endl;

                network.addEdge(temp, j, INF);
                //cout<<"added edge "<<temp<<" to "<<temp<<" = "<<matches[j][k]<<" between "<<j <<" and "<<k<<endl;
                network.addEdge(temp, k, INF);
               // cout<<"added edge "<<source<<" to "<<temp<<" = "<<matches[j][k]<<" between "<<j <<" and "<<k<<endl;
                //cout<<endl;
                temp++;
            }
        }

        bool ans = true;
        for(int j = 0; j<n;j++){
            if( i == j)
                continue;
            int maxWin = teams[i].w + teams[i].r - teams[j].w;
            if(maxWin < 0){
                ans = false;
                break;
            }
               
            //cout<<j<<" max win "<<maxWin<<endl;
            network.addEdge(j, target, maxWin);
        }

        EdmonKarp edk(network, source, target);

        //cout<<edk.maxFlow()<<endl;
        if(ans)
        for(int i = n+2 ; i<temp;i++)
            ans &= network.saturated(source, i);

        //if(ans)
             //cout<<teams[i].name<<" not eliminated"<<endl;
        if(!ans) 
            cout<<teams[i].name<<" eliminated"<<endl;
    
    }

}

