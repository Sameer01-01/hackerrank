#include <bits/stdc++.h>

using namespace std;

const int MOD=1000000007;
int N, Q;
vector<int> adj[200001];
vector<int> adj2[200001];
int P[18][200001];
int depth[200001];
int in[200001];
int out[200001];
int now;
int A[400001];
int mul[200001];
long long sum[200001];

void dfs(int u, int p)
{
    P[0][u]=p;
    for(int i=1; i<18; i++)
        P[i][u]=P[i-1][P[i-1][u]];
    in[u]=++now;
    for(auto& v: adj[u]) if(v!=p)
    {
        depth[v]=depth[u]+1;
        dfs(v, u);
    }
    out[u]=now;
}

int lca(int u, int v)
{
    if(depth[u]<depth[v])
        swap(u, v);
    for(int i=17; i>=0; i--) if(depth[P[i][u]]>=depth[v])
        u=P[i][u];
    if(u==v)
        return u;
    for(int i=17; i>=0; i--) if(P[i][u]!=P[i][v])
        u=P[i][u], v=P[i][v];
    return P[0][u];
}

int dfs2(int u, long long tot)
{
    int ret=0;
    sum[u]=u*mul[u];
    for(auto& v: adj2[u])
    {
        ret=(ret+dfs2(v, tot))%MOD;
        sum[u]+=sum[v];
    }
    for(auto& v: adj2[u])
        ret=(ret+1LL*((tot-sum[v])%MOD)
                    *(sum[v]%MOD)%MOD
                    *(depth[v]-depth[u])%MOD)%MOD;
    return ret;
}

int main()
{
    scanf("%d%d", &N, &Q);
    for(int i=0; i<N-1; i++)
    {
        int a, b;
        scanf("%d%d", &a, &b);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    dfs(1, 1);
    while(Q--)
    {
        int K;
        scanf("%d", &K);
        long long tot=0;
        for(int i=0; i<K; i++)
            scanf("%d", A+i), mul[A[i]]=1, tot+=A[i];
        sort(A, A+K, [](int a, int b) {
            return in[a]<in[b];
        });
        for(int i=0; i<K-1; i++)
            A[i+K]=lca(A[i], A[i+1]);
        sort(A, A+2*K-1);
        int M=unique(A, A+2*K-1)-A;
        sort(A, A+M, [](int a, int b) {
            return out[a]-in[a]>out[b]-in[b];
        });
        int root=A[0];
        map<int, int> m;
        m[in[root]]=root;
        for(int i=1; i<M; i++)
        {
            int u=A[i];
            auto it=m.upper_bound(in[u]);
            assert(it!=m.begin());
            --it;
            int p=it->second;
            adj2[p].push_back(u);
            //printf("%d -> %d\n", p, u);
            m[in[u]]=u;
            if(out[u]<out[p] && (!m.count(out[u]+1) || P[0][m[out[u]+1]]!=p))
                m[out[u]+1]=p;
        }
        printf("%d\n", dfs2(root, tot));
        for(int i=0; i<M; i++)
            adj2[A[i]].clear(), mul[A[i]]=0;
    }
    return 0;
}



