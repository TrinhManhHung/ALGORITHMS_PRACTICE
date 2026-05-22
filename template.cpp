#include <bits/stdc++.h>
using namespace std;

#define endl "\n"
#define ll long long
#define fi first
#define se second
#define sz size()
#define pb push_back
#define vi vector<int>
#define vll vector<ll>
#define vvi vector<vector<int>>
#define pii pair<int, int>
#define pll pair<ll, ll>
#define FOR(i, a, b) for(int i =a; i <=b; i++)
#define FOD(i, a, b) for(int i =a; i >=b; i--)
#define INF INT_MAX
#define MASK(i) (1LL << (i))
#define BIT(x, i) (((x) >> (i)) & 1)
#define COUNT_ONE(x) __builtin_popcountll((x))
#define COUNT_LEAD_ZERO(x) __builtin_clzll((x))
#define COUNT_TRAIL_ZERO(x) __builtin_ctzll((x))
#define SHOW1(x) cout << #x << " = " << (x) << endl << flush
#define SHOW2(x, y) cout << #x << "=" << (x) << " " << #y << "=" << (y) << endl << flush
#define faster() ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

const ll MOD = 998244353;
const int maxn = 200000;

//Modulo
ll addMod(ll a, ll b){
	return (a + b) % MOD;
}

ll subMod(ll a, ll b){
	return (a - b + MOD) % MOD;
}

ll mulMod(ll a, ll b){
	if(b == 0) return 0;
	if(b == 1) return a;
	ll half = mulMod(a, b / 2);
	
	ll res = (half + half) % MOD;
	if(b & 1) res = (res + a) % MOD;
	
	return res;
}

ll powMod(ll a, ll b){
	if(b == 0) return 1;
	ll half = powMod(a, b / 2);
	
	ll res = half * half % MOD;
	if(b & 1) res = res * a % MOD;
	
	return res;
}

//To Hop Chap k Cua n
const int maxn = 200005;
const ll MOD = 998244353;

ll frac[maxn], finv[maxn]; //frac[n]: n!, finv[n]: (n!)^(MOD-2)

ll powMod(ll x, ll k){
    if(k == 0) return 1;
    ll half = powMod(x, k/2);
    half = (half * half) % MOD;
    return (k & 1) ? (half * x) % MOD : half;
}

void prepare(){
	frac[0] = 1;
	FOR(i, 1, maxn-1) frac[i] = frac[i-1] * i % MOD;
	finv[maxn-1] = powMod(frac[maxn-1], MOD-2); finv[0] = 1;
	FOD(i, maxn-2, 1) finv[i] = finv[i+1] * (i+1) % MOD;  
}

ll comb(int k, int n){ //C(k, n)
	return k > n? 0 : frac[n] * finv[k] % MOD * finv[n-k] % MOD;
}
//Chia Can
int n;
int block = sqrt(n);

bool cmp(pair<pii, int> a, pair<pii, int> b){
 
    int pa = a.fi.se / block;
    int pb = b.fi.se / block;
    
    if(pa != pb) return pa < pb;
    else{
        return (pa % 2 == 0? a.fi.fi < b.fi.fi : a.fi.fi > b.fi.fi);
    } 
}

//Fenwick Tree
struct FenwickTree{
	int n;
	vll tree;
	
	FenwickTree(int _n){
		n = _n;
		tree.assign(n + 5, 0);
	}
	
	void update(int pos, int val){
		for(; pos <= n; pos += (pos & -pos)){
			tree[pos] += val;
		}
	}
	
	ll get(int pos){
		ll res = 0;
		for(; pos > 0; pos -= (pos & -pos)){
			res += tree[pos];
		}
		return res;
	}
};

//Segment Tree
struct SegmentTree{
	int n;
	vll tree, lazy;
	
	SegmentTree(int _n){
		n = _n;
		tree.assign(4*n + 5, 0);
		lazy.assign(4*n + 5, 0);
	}
	
	void pushDown(int id, int tl, int tr){
		if(lazy[id] == 0) return;
		
		int tm = (tl + tr) >> 1;
		
		tree[2*id] += lazy[id] * (tm - tl + 1);
		tree[2*id+1] += lazy[id] * (tr - tm);
		lazy[2*id] += lazy[id];
		lazy[2*id+1] += lazy[id];
		
		lazy[id] = 0;
	}
	
	void update(int id, int tl, int tr, int u, int v, int val){
		if(tl > v || tr < u) return;
		if(u <= tl && tr <= v){
			tree[id] += val * (tr - tl + 1);
			lazy[id] += val; 
			return;
		}
		
		pushDown(id, tl, tr);
		int tm = (tl + tr) >> 1;
		update(2*id, tl, tm, u, v, val);
		update(2*id+1, tm+1, tr, u, v, val);
		
		tree[id] = tree[2*id] + tree[2*id+1]; //pushUp
	}
	
	ll get(int id, int tl, int tr, int u, int v){
		if(tl > v || tr < u) return 0ll;
		if(u <= tl && tr <= v) return tree[id];
		
		pushDown(id, tl, tr);
		int tm = (tl + tr) >> 1;
		ll L = get(2*id, tl, tm, u, v);
		ll R = get(2*id+1, tm+1, tr, u, v);
		
		return L + R;
	}
};

//Hash (String s danh so tu 1)
int n;

const int maxn = 200000;
const int MOD = 1e9 + 7;
const int BASE = 256;

string s;
ll hs[maxn + 5]; //hs[i] = ma hash cua xau s[1...i]
ll pw[maxn + 5]; //pw[i] = 256^i
ll invPw[maxn + 5]; //invPw[i] = pw[i] ^ (MOD - 2)

ll powMod(ll a, ll b){
	if(b == 0) return 1;
	ll half = powMod(a, b / 2);
	
	ll res = half * half % MOD;
	if(b & 1) res = res * a % MOD;
	
	return res;
}

void prepare(){
    pw[0] = 1;
    FOR(i, 1, maxn) pw[i] = pw[i-1] * BASE % MOD;
    invPow[maxn] = powMod(pw[maxn], MOD-2);
    FOD(i, maxn, 1) invPow[i-1] = invPow[i] * BASE % MOD;
}

ll calcHash(string s){
	FOR(i, 1, s.sz - 1){
		hs[i] = hs[i-1] + s[i] * pw[i-1];
		hs[i] %= MOD;
	}
	return hs[s.sz - 1];
}

ll calcSegmentHash(int l, int r){
	return (hs[r] - hs[l-1] + MOD) % MOD * invPw[l-1] % MOD;
}

//DSU: Moi TPLT Luu Nut Cha, So Luong Dinh, So Luong Canh
struct DSU{
	int n;
	vi parent, size, edge;
	
	DSU(int _n){
		n = _n;
		parent.assign(n+5, 0);
		size.assign(n+5, 0);
		edge.assign(n+5, 0);
		FOR(i, 1, n){
			parent[i] = i;
			size[i] = 1;
			edge[i] = 0;
		}
	}
	
	int Find(int u){
		if(parent[u] == u) return u;
		return parent[u] = Find(parent[u]);
	}
	
	bool Union(int u, int v){
		u = Find(u);
		v = Find(v);

		if(u == v){
			edge[u]++;
			return false;
		}
		if(size[u] > size[v]){
			parent[v] = u;
			
			size[u] += size[v];
			edge[u] += edge[v] +1;
			
			size[v] = 0;
			edge[v] = 0;
		} else{
			parent[u] = v;
			
			size[v] += size[u];
			edge[v] += edge[u] +1;
			
			size[u] = 0;
			edge[u] = 0;
		}
		return true;
	}

};

//Dijkstra
int n;
vector<pii> adj[1005];

int dijkstra(int sta, int end){
	vi dist(n+1, INF);
	
	priority_queue<pii, vector<pii>, greater<pii>> Q; //<dist, vertex>
	dist[sta] = 0;
	Q.push({0, sta});
	
	while(!Q.empty()){
		auto top = Q.top(); Q.pop();
		
		int cur_dist = top.fi;
		int u = top.se;
		
		for(auto it : adj[u]){
			int v = it.fi;
			int w = it.se;
			if(dist[v] > dist[u] + w){
				dist[v] = dist[u] + w;
				Q.push({dist[v], v});
			}
		}		
	}
	
	return dist[end];	
}

//Topo Sorting: Ap Dung Cho Directed Acyclic Graph
int n;
vi in_degree, topo;
vvi adj;

void kahn(){
	queue<int> Q;
	
	FOR(u, 1, n){
		if(in_degree[u] == 0) Q.push(u);
	}
	
	while(!Q.empty()){
		auto u = Q.front(); Q.pop();
		topo.pb(u);
		for(auto v : adj[u]){
			in_degree[v]--;
			if(!in_degree[v]) Q.push(v);
		}
	}
}

//Liet ke canh cau
int n, m;
vi adj[maxn+5];
int num[maxn+5], low[maxn+5]; //low[v]: chi so cua dinh nho nhat co canh noi den cay con cua v
int cnt, cntBridge;

void DFS(int u, int parent){
    low[u] = n+1; //duong vo cuc
    num[u] = ++cnt;

    for(int v : adj[u]){
        if(v == parent) continue;

        if(!num[v]){
            DFS(v, u);

            if(low[v] > num[u]){
                cntBridge++;
            }

            low[u] = min(low[u], low[v]);
        } else{
            low[u] = min(low[u], num[v]);
        }
    }
}

//Tim LCA
const ll MOD = 1e9 + 7;
const int maxn = 70000;
const int LOG = 18;

int n, q;
int par[maxn+5][LOG+5]; //parent[v][p]: cha thu 2^p cua dinh v
int depth[maxn+5];
vvi adj;
bool used[maxn+5];

void DFS(int u){
    used[u] = true;
    for(auto v : adj[u]){
        if(!used[v]){
            par[v][0] = u;
            depth[v] = depth[u] + 1;
            DFS(v);
        }
    }
}

void prepare(){
     FOR(p, 1, LOG){
        FOR(v, 1, n){
            par[v][p] = par[par[v][p-1]][p-1];
        }
     }
}

//depth[u] > depth[v]
int lca(int u, int v){
    if(depth[u] < depth[v]) return lca(v, u);

    //can bang do sau
    FOD(p, LOG, 0){
        if(depth[par[u][p]] >= depth[v]) u = par[u][p];
    }

    if(u == v) return u;
    //tim nut cha
    FOD(p, LOG, 0){
        if(par[u][p] != par[v][p]){
            u = par[u][p];
            v = par[v][p];
        }
    }

    return par[u][0];
}


//Nhan Ma tran
const int MAXSZ = 25;

struct Matrix{
    int n, m;
    ll d[MAXSZ+5][MAXSZ+5];

    Matrix(int _n = 0, int _m = 0){
        n = _n, m = _m;
        memset(d, 0, sizeof(d));
    }

    Matrix operator * (const Matrix &o) const{
        int x = n, y = m, z = o.m;
        Matrix res(x, z);

        FOR(i, 0, x-1) FOR(j, 0, z-1)
            FOR(k, 0, m-1){
                res.d[i][j] += d[i][k] * o.d[k][j];
                res.d[i][j] %= MOD;
            }
        return res;
    }
};

Matrix powMod(Matrix &o, ll p){
    Matrix res(o.n, o.m);
    FOR(i, 0, o.n-1) res.d[i][i] = 1;

    if(p == 0){
        return res;
    }
    
    Matrix half = powMod(o, p / 2);
    res = half * half;
    if(p & 1) res = res * o;
    return res;
}

//Liet Ke Nhanh Cac Tap Hop Con Cua Mask Theo Thu Tu Giam Dan
void listAllSubset(int mask){
	for(int sub = mask; sub >= 0; sub--){
		sub &= mask;
	}
}
//Duyet Nhanh Cac Bit 1 Cua Mask
void listBitOne(int mask){
	for(ll m = mask; m > 0; m = m & (m-1)){
		int b = __builtin_ctzll(m);
	}
//Trie
const ll MOD = 1e9+7;
const int N = 1e6+5;
const int ALPHABET_SIZE = 26;
 
struct Node{
    int cntRev;
    unordered_map<char, Node*> child;
    Node(){
        cntRev = 0;
    }
};
 
Node nodes[N];
int trieNodeCount;
Node* root;
 
Node* createNode(){
    return &nodes[trieNodeCount++];
}
 
void addString(const string &s){
    Node* p = root;
 
    for(char c : s){
        if(p->child.find(c-'a') == p->child.end()) p->child[c-'a'] = createNode();
        p = p->child[c-'a'];
        p->cntRev++;
    }
}

int main(){
    faster();
    int t = 1;
//    cin >> t;
    while(t--){
        HuyenMay();
    }
    return 0;
}