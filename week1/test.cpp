#include <iostream>
#include <vector>
#include <stack>
using namespace std;
vector<vector<int> > g;
vector<int> tin,tout,vis;
int curTime=1;
stack<int> order;
bool dfs(int node)
{
    vis[node]=1;
    tin[node]=curTime++;
    for(int e:g[node])
    {
        if(vis[e])
        {
            if(tout[e]==0)
                return false;
            continue;
        }
        if(!dfs(e))
            return false;
    }
    tout[node]=curTime++;
    order.push(node);
    return true;
}
int main()
{
    int n,d;
    cin>>n>>d;
    g.resize(n);
    for(int i=0;i<d;++i)
    {
        int s1,s2;
        cin>>s1>>s2;
        --s1;--s2;
        g[s2].push_back(s1);
    }
    tin.assign(n,0);
    tout.assign(n,0);
    vis.assign(n,0);
    for(int i=0;i<n;++i)
    {
        if(!vis[i])
        {
            if(!dfs(i))
            {
                cout<<"not possible\n";
                return 0;
            }
        }
    }
    while(!order.empty())
    {
        cout<<order.top()+1<<' ';
        order.pop();
    }
    return 0;
}