#include <bits/stdc++.h>
using namespace std;

/**
 * @brief
 *
 * @param u starting node
 * @param adj is the adjaceny list of the graph
 * @param visited vector tracks if a node is visited
 *              value 0 for not visited, 1 for visited, 2 for added in topoligical order
 * @param order is resulting topogical order
 * @return true if the graph don't have any cycle
 * @return false if the graph have a cycle
 */
bool dfs(int u, vector<vector<int>> &adj, vector<int> &visited, vector<int> &order)
{
    visited[u] = 1; // seen the node

    bool acyclic = true;

    for (int &v : adj[u])
    {
        if (visited[v] == 1)
        { // already visited
            // cout<<"cycle detected form "<<u<<" -> "<<v<<endl;
            return false;
        }

        if (!visited[v])
            acyclic &= dfs(v, adj, visited, order);
    }

    visited[u] = 2; // added to topological order
    order.push_back(u);
    return acyclic;
}

vector<int> topsort(int n, vector<vector<int>> &adj, vector<int> &visited)
{
    vector<int> order;
    for (int i = 1; i <= n; i++)
    {
        if (!visited[i])
        {
            bool ok = dfs(i, adj, visited, order);
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

int main(int argc, char const *argv[])
{
    int n;
    cin >> n; // number of steps needed for car assembly
    int d;
    cin >> d;   // number of dependencies
    int s1, s2; //  step s1 dependent of step s2

    vector<vector<int>> adj(n + 1);

    for (int i = 0; i < d; i++)
    {
        cin >> s1 >> s2;
        adj[s1].push_back(s2);
    }

    vector<int> visited(n + 1, 0);
    vector<int> order = topsort(n, adj, visited);

    if (order.size() == 0)
    {
        cout << "Not possible" << endl;
        return 0;
    }

    for (int i : order)
        cout << i << ' ';
    cout << endl;

    return 0;
}
