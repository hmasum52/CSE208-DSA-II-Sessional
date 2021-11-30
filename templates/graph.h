// cpp header file tutorial
// https://www.youtube.com/watch?v=9RJTQmK0YPI
#pragma once // header guard: prevents including same header file mulitiple time

#include <iostream>
#include <vector>
#include <queue>
#include <cstdarg>
#include <cstring>
using namespace std;

/**
 * @brief prints the message in the console.
 * tutorial: https://www.youtube.com/watch?v=S-ak715zIIE
 * 
 * @param format is the message to be printed in the console.
 */
void log(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}

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
        if (!visited.size())
            visited.resize(n + 1);
        visited.assign(n + 1, 0);
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
        adj.resize(n + 1);

        //log("graph is created of size: " + to_string(n));
    }

    /**
     * @brief add and edge to a undirected graph
     * the class asumes edge starts from 1
     * 
     * @param u starting node of the edge.
     * @param v ending node of the edge.
     */
    void addDirectedEdge(int u, int v)
    {
        adj[u].push_back(v);
    }

    /**
     * @brief add and edge to a directed graph
     * the class asumes edge starts from 1
     * 
     * @param u starting node of the edge.
     * @param v ending node of the edge.
     */
    void addUndirectedEdge(int u, int v)
    {
        // cout<<"adding undireced edge "<<u<<" -> "<<v<<endl;
        adj[u].push_back(v);
        adj[v].push_back(u);
        // cout<<"edge added"<<endl;
    }

    /**
     * @brief start traversing all the nodes from given source
     * 
     * @param source start node of the bfs
     */
    void bfs(int source)
    {
        // init visited
        initVisited();

        queue<int> q;
        q.push(source);

        //start bfs
        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            for (int &v : adj[u])
            {
                if (!visited[v])
                {
                    visited[v] = 1;
                    q.push(v);
                }
            }
        }
    }

    void dfs(int source)
    {
        initVisited();
        dfs_util(source);
    }

    void dfs_util(int u)
    {
        visited[u] = 1; // mark as seen

        for (int &v : adj[u])
        {
            if (!visited[v])
                dfs_util(v);
        }

        visited[u] = 2; // mark explored
    }

    bool detectCycle()
    {
        return true;
    }

    bool cycle_util(int u)
    {
        visited[u] = 1; // seen the node

        bool acyclic = true;

        for (int &v : adj[u])
        {
            if (visited[v] == 1) // check if found backedge
            {                    // already visited
                //cout<<"cycle detected form "<<u<<" -> "<<v<<endl;
                return false;
            }

            if (!visited[v])
                acyclic &= cycle_util(v);
        }

        visited[u] = 2;
    }

    /**
     * @brief check bipartite assuming that
     * the vertex starts from 1.
     * 
     * @return true is the graph is bipartite.
     * @return false otherwise.
     */
    bool isBipartite()
    {
        initVisited();
        bool ok = true;
        for (int i = 1; i <= n; i++)
        {
            if (!visited[i])
                ok &= bipartiteUtil(i);
        }

        return ok;
    }

    /**
     * @brief use bfs to check if the graph is bipartite.
     * If a graph has an edge in the same layer in its bfs layers then 
     * the cycle contains an odd cycle. Hence the graph is not bipartite as
     * bipartite graph can't contain any odd cycle.
     * 
     * @param src 
     * @return true if the graph is bipartite.
     * @return false otherwise.
     */
    bool bipartiteUtil(int src)
    {
        queue<int> q;
        q.push(src);

        vector<int> color(n + 1, -1);
        color[src] = 1; // 1 for blue
        visited[src] = 1;

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            for (int &v : adj[u])
            {

                if (v == u || color[u] == color[v]) // self loop or detect odd cycle
                {
                    //log("odd cycle found from %d to %d\n", u, v);
                    return false;
                }

                if (color[v] == -1)
                {
                    //log("edge %d -> %d\n", u, v);
                    color[v] = (color[u] + 1) % 2;
                    visited[v] = 1;
                    q.push(v);
                }
            }
        }

        return true;
    }

    /**
     * @brief use dfs to find the order.
     * while running dfs it looks for a back edge to detect cycle.
     * A graph with cycle don't have any topological order.
     * 
     * @return vector<int> the topogical order.
     * return an empty list if the graph have a cycle.
     */
    vector<int> topsort()
    {
        initVisited();

        vector<int> order;
        for (int i = 1; i <= n; i++)
        {
            if (!visited[i])
            {
                bool ok = topsortUtil(i, order);
                // cout<<"at node "<<i<<" cycle detected: "<<ok<<endl;
                if (!ok)
                {
                    order.clear();
                    // cout<<"cycle detected"<<endl;
                    break;
                }
            }
        }
        return order;
    }

    /**
     * @brief use dfs to find the topological order
     * 
     * @param u starting node
     * @param adj is the adjaceny list of the graph
     * @param visited vector tracks if a node is visited
     *              value 0 for not visited, 1 for visited, 2 for added in topoligical order
     * @param order is resulting topogical order
     * @return true if the graph don't have any cycle
     * @return false if the graph have a cycle
     */
    bool topsortUtil(int u, vector<int> &order)
    {
        visited[u] = 1; // seen the node

        bool acyclic = true;

        for (int &v : adj[u])
        {
            if (visited[v] == 1)
            { // already visited
                //cout<<"cycle detected form "<<u<<" -> "<<v<<endl;
                return false;
            }

            if (!visited[v])
                acyclic &= topsortUtil(v, order);
        }

        visited[u] = 2; // added to topological order
        order.push_back(u);
        return acyclic;
    }
};