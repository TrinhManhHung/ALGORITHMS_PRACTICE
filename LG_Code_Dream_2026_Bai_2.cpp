/*
Đề bài: Dãy con xoay vòng không giảm dài nhất
Cho một danh sách gồm n chuỗi ký tự (s_1, s_2, ..., s_n). 
Với mỗi chuỗi s_i, bạn có thể chọn một biến thể xoay vòng (rotated version) bất kỳ của nó.
Ví dụ: Với chuỗi abcd, các biến thể xoay vòng là: abcd, bcda, cdab, dabc.
Yêu cầu:Hãy chọn ra một dãy con (subsequence) từ danh sách $n$ chuỗi ban đầu: (s{i1}, s{i2}, ..., s{ik}) với 1 < i_1 < i_2 < ... < i_k < n, 
sao cho sau khi chọn một biến thể xoay vòng tương ứng cho mỗi chuỗi, ta được một dãy mới thỏa mãn:
Các chuỗi trong dãy được chọn phải xếp theo thứ tự từ điển không giảm.
Mục tiêu: Tìm độ dài $k$ lớn nhất có thể.
*/

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

const ll MOD = 1e9 + 7;
const int N = 2e5 + 5;
const int BASE = 256;
const int ALPHABET_SIZE = 26;

struct Rotation{
    int id, start, len;

    Rotation(int _id, int _start, int _len){
        id = _id; start = _start; len = _len;
    }
};

struct FenwickTree{
    int n;
    vi tree;
    FenwickTree(int _n){
        n = _n;
        tree.assign(n+5, 0);
    }

    void update(int pos, int val){
        for(int i=pos; i <=n; i += (i & -i)) tree[i] = max(tree[i], val);
    }

    int getMax(int pos){
        int res = 0;
        for(int i =pos; i > 0; i -= (i & -i)) res = max(res, tree[i]);
        return res;
    }
};

vvi pref; //hashing
ll pw[N], invPw[N];

int n;
vector<string> s, dbl;
vector<Rotation> rots; vvi belong;

ll powMod(ll &a, ll p){
    if(p == 0) return 1;
    ll half = powMod(a, p/2);
    ll res = half * half % MOD;
    if(p & 1) res = res * a % MOD;
    return res;
}

void prepare(){
    pw[0] = 1;
    FOR(i, 1, N-1) pw[i] = pw[i-1] * BASE % MOD;
    invPw[N-1] = powMod(pw[N-1], MOD-2);
    FOD(i, N-1, 1) invPw[i-1] = invPw[i] * BASE % MOD;

    for(string &ss : dbl){
        vi hs(ss.sz); hs[0] = ss[0];
        for(int i =1; i <ss.sz; i++) hs[i] = (hs[i-1] + ((ss[i] * pw[i]) % MOD)) % MOD;
        pref.pb(hs);
    }
}

ll getHash(int id, int l, int r){
    if(l > r) return 0;
    ll res = pref[id][r];
    if(l > 0) res = (res - pref[id][l-1] + MOD) % MOD * invPw[l] % MOD;
    return res;
}

//vi tri dau tien pos ma x[pos] != y[pos]
int lcp(const Rotation &x, const Rotation &y){
    int L = 0, R = min(x.len, y.len);
    int pos = -1;

    while(L <= R) {
        int M = (L + R) >> 1;
        if(getHash(x.id, x.start, x.start+M-1) == getHash(y.id, y.start, y.start+M-1)){
            pos = M;
            L = M + 1;
        } 
        else R = M - 1;
    }
    
    return pos + 1;
}

bool cmpRotation(const Rotation &x, const Rotation &y){
    int pos = lcp(x, y);
    if(pos >= min(x.len, y.len)) return x.len < y.len;
    
    char cx = dbl[x.id][x.start+pos-1];
    char cy = dbl[y.id][y.start+pos-1];

    return cx < cy;
}

void HuyenMay(){  
    cin >> n;
    s.resize(n), dbl.resize(n), belong.assign(n, vi());
    
    FOR(i, 0, n-1) {
        cin >> s[i], dbl[i] = s[i] + s[i];

        int len = s[i].sz;
        for(int j =0; j <len; j++){
            rots.pb(Rotation(i, j, len));
            belong[i].pb(rots.sz - 1);
        }
    }

    // for(const auto &x : rots){
    //     cout << x.id << " " << x.start << endl;
    // }
    // //prepare hashing
    prepare();
    // int tmp = lcp(rots[3], rots[6]);
    // SHOW1(tmp);

    vi ord(rots.sz);
    FOR(i, 0, rots.sz-1) ord[i] = i;

    sort(ord.begin(), ord.end(), [&](int &a, int &b){
        return cmpRotation(rots[a], rots[b]);
    });
    // for(auto &x : ord) cout << x << " ";

    vi rankRots(ord.sz);
    int rankId = 1;
    rankRots[ord[0]] = rankId;
    FOR(i, 1, ord.sz-1){
        int u = ord[i], v = ord[i-1];
        if(lcp(rots[u], rots[v]) < max(rots[u].len, rots[v].len)) rankId++;
        rankRots[u] = rankId;
    }

    FenwickTree fenTree(N);
    int res = 1;
    for(int i =0; i <n; i++){
        vector<pii> upd;
        for(auto &x : belong[i]){
            int rank = rankRots[x];
            int max_sequence = fenTree.getMax(rank) + 1;
            res = max(res, max_sequence);
            upd.pb({rank, max_sequence});
        }

        for(auto &[x, y] : upd){
            fenTree.update(x, y);
        }
    }

    cout << res << endl;
}

int main(){
    faster();
    int t = 1;
    // cin >> t; 
    while(t--){
        HuyenMay();
    }
    return 0;
}
/** HungLam
	  (\(\
	 ( 0.0)
	o_(")(")
**/
/**
⣿⣛⢿⣛⢿⡙⣏⢻⡙⠏⠭⣉⠇⡹⢌⡣⡍⢭⠩⢥⢩⢡⡉⡜⣩⠈⠥⡉⢌⠡⡉⠎⡍⢭⠩⢍⠩⢯⠹⣍⢯⡙⢭⢫⡝⣭⠩⡉⢍⠩⡉⢍⡉⢣⠙⡌⢣⠉⢎⡝⢬⢣⡝⣬⢍⡭⣩⡙⣍⣋⡝⢣⠛⠬⢡⠉⠬⡑⢫⢻⡹⣍⡛⢿⣿⡏⠥⡉⢌⠡
⣷⣫⢾⡹⢮⡽⡘⢡⠘⣌⠲⣄⢺⡵⣋⠷⣹⢎⡗⣆⠒⡂⠖⠬⠤⡙⠤⡑⢌⠢⡑⢊⡜⢢⡝⢦⠳⣤⢣⠴⣠⢌⣄⢢⡑⡠⢃⠜⡠⢃⠜⡠⠘⡄⢣⠘⠤⣉⣦⢞⣭⣣⣼⡴⣎⣶⣥⣽⣦⣷⣼⢧⣛⣤⢂⠍⠤⡁⢎⡡⢓⠬⡙⢎⠳⢋⢦⠱⣌⢂
⣷⣫⢿⡝⣧⡓⣌⢡⠋⡔⢣⢎⡷⣱⡙⢮⠵⣎⠽⣎⠳⣌⠘⡄⢣⠘⠤⡑⢌⠢⣁⠣⡜⡡⠞⡥⢛⡔⢫⠞⡱⢎⡜⣣⢝⡱⣋⠞⡡⢊⠔⡡⢃⠜⣠⡿⣞⣿⡾⣿⢿⣿⣿⣽⣿⣿⢿⣟⣿⡽⣯⢿⣯⣟⡯⣞⠤⡑⢌⡒⢏⠶⡹⢎⠷⣊⢶⠡⢆⠂
⣿⡭⣗⡻⣞⠽⣈⢇⡛⡜⢣⢏⠶⣡⠏⣝⠺⣌⠳⣜⢣⡜⣰⠘⡤⣉⠆⡱⢈⠒⡄⠳⡐⢥⢫⡔⢣⠜⣥⢮⡵⣎⣴⡡⢎⡱⢡⠊⡔⢡⠊⣔⣥⣾⢿⣹⣽⣶⢿⣽⡾⣷⣿⣿⣿⣻⡿⣯⢿⡽⣯⣟⡾⣽⣻⡽⣾⣑⠪⡔⢪⡒⣥⢊⢦⡡⢎⡲⢈⠆
⣯⢳⡽⣫⣝⠲⠠⠎⡴⣉⠶⣈⠧⣡⢋⠬⡓⣌⠳⣌⠣⡜⣡⠛⡴⢉⠆⡱⢈⠒⡌⠣⡍⢆⡻⣜⢧⣛⡬⢷⢻⡽⣞⡽⣳⣌⢣⡙⣄⢣⣼⣟⣷⣯⣿⣿⣿⣾⣯⣷⣿⣿⣽⣯⣟⠿⣽⢏⣿⣱⢳⢮⢽⡘⢧⣻⡱⢻⢧⡙⢦⣡⢁⢎⡐⢩⠘⡄⠣⠌
⣯⢳⡽⣱⢏⣇⢣⠚⡴⣡⢾⣷⣿⣶⣿⣾⣷⣬⣷⣌⠳⡘⢤⢋⠴⡁⢎⠰⡁⢎⡐⢣⠜⡢⢝⣮⢳⣹⣾⣶⣦⣶⣤⣦⣥⣬⣡⣃⢬⣿⣿⣾⣿⣿⣿⣿⣿⡿⠿⠻⠟⠿⠿⢿⣿⣿⡽⣞⡷⣯⣟⣾⣳⣟⣧⢷⣹⢧⣯⡝⡦⢹⢎⠮⣙⠧⣛⠜⡱⢈
⡷⣏⠷⣭⢚⡬⢣⢏⠶⣡⢟⡼⣳⣟⣾⣽⣯⢿⣿⣿⢡⡙⢆⡑⢪⠐⡌⢢⠑⢢⠘⠤⢊⡕⢺⡜⣧⢏⡷⣭⢯⡝⣯⡝⢯⡻⡝⣱⣿⣿⣿⣿⣿⣿⣿⣿⠣⠐⡁⠂⠌⡐⢀⠂⡘⢿⣟⣯⣟⣷⣻⣞⣷⣻⣞⣯⣟⡿⣾⣽⣳⡅⢺⢦⡱⣎⡔⢣⠔⡡
⢝⡾⣹⢎⡳⣚⠵⣊⢿⡱⢎⢷⣻⡾⣽⡾⣯⣻⣿⣿⢠⡙⠦⣙⢢⠑⡌⢢⠉⢆⡉⢆⠣⡘⢥⣛⢾⣿⢿⡿⣿⣿⣳⡽⣣⢗⣸⣿⣿⣿⣿⣿⣿⣿⣿⠃⠄⠡⢀⠁⢂⠐⡀⢂⠐⡘⣿⢿⣞⣯⣷⣻⣞⣷⣻⢾⣽⣻⢷⣯⢷⣏⡆⢯⠱⣋⢞⡳⢎⡵
⢎⡷⣣⢞⡱⢫⡜⣡⣏⢷⣩⢺⣵⡿⣯⢿⣳⡽⣷⣿⢠⡙⢢⡑⢪⠐⡌⢢⠉⢆⡘⢄⠣⡑⢦⡙⣾⣿⢯⣟⡷⣿⣻⡵⣋⠦⣿⣿⣿⣿⣿⣿⣿⣽⡃⠌⠠⢁⠂⠌⡀⢂⡐⣀⠂⠄⢻⡏⣿⣽⣞⣷⣻⢾⡽⣯⣿⡽⣿⢾⣟⡾⣵⢪⡷⣷⠾⣶⢷⡾
⡿⣼⢳⢎⡱⢣⢞⡵⣮⣟⣯⢧⠻⣿⡽⣿⣳⣟⣿⣿⢠⡙⢆⡙⢦⠑⡌⢢⠉⢆⡘⢄⠣⣘⠢⡑⣯⣿⣻⢾⡽⣿⣟⠶⣁⢻⣿⣿⣿⣿⣿⣿⠇⡟⢣⠈⡐⠠⢈⠐⣈⠛⠭⡉⠏⡙⠺⡟⢿⣽⡾⣿⡽⣯⣟⡷⣿⣿⣻⢿⣯⣟⣧⠧⣿⣾⡿⣜⠧⢂
⡷⣭⣛⢎⡖⣹⢾⡽⣞⣿⣞⣯⢷⡹⣟⣯⣷⣞⣿⣷⢢⡙⢆⡙⢢⠑⡌⢢⠉⢆⡘⢄⠣⢄⠣⡘⡼⣿⠽⣯⢻⣿⣽⢣⡃⢾⣿⣿⣿⣿⣿⣟⣈⠧⡀⠡⢀⠁⠂⠔⡠⢉⠐⣀⣂⠄⠡⣟⢸⣹⢿⣿⣻⡷⣯⣟⣿⣿⣻⣿⣯⢿⣞⡯⢿⣽⡿⣜⡃⠆
⣻⢶⡙⡮⠜⣽⣫⢿⣟⣷⣻⣞⣯⢷⡹⣿⢾⡽⣿⣷⢢⡙⢆⡙⢦⠑⡌⢢⠉⢆⡘⢄⠣⢌⠢⢡⢹⣯⢻⡜⣯⣿⢯⡳⢌⣻⣿⣿⣿⣿⣿⠏⣽⣷⣧⠐⠠⢈⠐⡈⠰⣬⡷⢶⣮⣾⣥⣻⡸⢼⢺⣿⣯⣟⣷⣻⣿⣿⣿⣽⣿⣟⡾⣝⣻⢾⣟⡰⡁⠎
⣯⢞⡵⡹⣘⣧⣟⣿⣻⣞⣷⣻⣞⣯⢷⡘⢯⣟⣿⡷⢢⡙⢦⡙⢦⠘⡄⢣⠘⠤⡘⢄⠣⢌⠢⡁⢾⣜⣧⡻⣵⡿⣿⡱⢂⢽⣿⣿⣿⡧⢿⠂⣛⣿⣿⠆⡁⢂⠐⠄⠋⠔⡈⢲⣿⣿⣿⣿⣿⣿⣼⣿⣯⣿⣞⣿⢆⢹⣿⣟⣿⣾⣻⣟⡼⣻⣷⠰⡁⠎
⣯⢾⡱⢣⣜⡾⣽⡾⣟⣾⣳⣟⡾⣽⣻⢮⡱⣞⣿⡿⢰⡉⢦⡙⢦⠘⡄⢣⠘⠤⡑⢌⠒⡌⢢⠑⢮⡿⣞⡷⣹⣿⣻⢥⠃⢾⣿⣿⣿⣿⠹⡇⢸⣍⢻⡱⢀⠂⡘⠠⢁⠂⠄⣳⢻⡿⣿⢿⣿⠣⢼⣿⡿⣞⣯⡿⣌⢺⣿⣿⣿⡷⣿⡽⣼⣻⣧⢣⠑⡌
⣟⡮⣝⢣⠾⣽⡿⣽⣻⣞⡷⣯⣟⡷⣯⢿⡵⡜⣿⣿⢰⡉⢦⡙⢦⠘⡄⢣⠘⠤⡑⢌⠲⣌⢣⠉⡌⣉⠡⣉⠳⣉⠙⡊⢱⢈⣿⣿⣿⣿⡷⡈⢄⠓⡦⠃⠄⠂⠄⡁⠂⠌⠠⢙⢦⠹⡱⢚⠏⡰⣻⣿⣿⢿⣿⠗⡬⢻⣿⣿⣿⢿⣽⣟⢶⣻⣽⢂⠣⡐
⡿⡼⣡⢏⠿⣽⡿⣽⣳⢯⣟⡷⣯⢿⡽⣯⣟⡾⡜⣿⢰⡉⢦⡙⢦⠘⡄⢣⠘⠤⡑⢌⠒⡌⢦⠑⡌⢄⠣⢄⠳⢠⠙⡄⢣⠘⣼⣿⣿⣿⣿⠐⣌⠳⣄⠡⢈⠐⠠⢀⠡⢈⠐⡠⢌⠡⢁⠊⠴⣡⣿⣿⣿⣿⡿⡘⣜⣽⣿⣿⣿⣯⢿⣟⡎⢙⠣⡉⠖⡡
⣛⡵⣫⢞⣽⡿⣽⣳⢯⣟⡾⣽⢯⡿⣽⣳⢯⡿⣵⢎⠳⡘⢦⡙⢆⠘⡄⢣⠘⠤⡑⢌⠢⡙⢤⠃⡜⢠⠃⡌⢢⢁⠚⡄⢣⠘⡰⣿⣿⣿⣿⡌⠄⠡⢈⠐⠠⢈⠐⡀⠂⢌⠒⡡⢊⠐⠠⢉⡲⠝⣰⣿⢿⡽⣣⣵⣿⣿⣿⣿⣿⢾⣿⣻⠆⡡⢆⠡⡃⠔
⣯⢳⡝⣮⣿⣻⡽⡏⠿⣾⡽⣯⢿⣽⣳⢯⡿⣽⣻⢮⢇⡙⢦⡙⢆⠈⠄⡁⢊⠐⡁⢊⠐⡁⢂⠡⠈⠄⠡⢈⠐⡈⢂⠘⡀⢃⠰⣿⣿⣿⣿⣷⡈⠐⠠⢈⠐⡀⠂⠄⡁⢂⠐⡀⠂⠌⡐⠠⢁⢲⡿⢫⣿⣽⣿⣿⣿⣿⣿⣿⣿⢯⣿⣯⡇⡘⢄⠣⡘⠄
⢯⡳⡝⣾⡷⣿⣽⣃⠳⣌⢻⣽⡻⢎⡝⢪⣽⣳⢯⣟⣯⡜⢢⡙⢆⠙⢢⠙⢢⠙⡰⢉⠎⡱⢉⠎⡱⢉⠎⡡⢋⠜⡡⢊⡑⢊⠱⡟⣿⣿⣿⣿⣿⣆⡁⢂⠒⠄⡁⢂⠐⡀⠂⠄⡁⠂⠄⡁⢎⡝⢲⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣳⣿⡳⠄⠦⠰⠄⢆
⣧⢹⢼⣿⣻⢷⣯⣇⢣⠘⠦⡘⠥⢃⠌⡳⣾⣽⣻⣞⣷⣛⢦⢉⠆⡘⠤⡉⢆⠱⣀⠣⡘⢄⠣⡘⢄⠣⡘⣤⢣⣜⡤⢃⡜⢠⢃⡇⢻⣿⣿⣿⣿⣿⣷⣄⠈⡐⢀⠂⡐⠠⢁⡂⢤⢃⠞⣩⠒⣌⢣⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⢯⣷⣟⡌⢢⠑⡌⢂
⢶⣩⣾⡿⣽⣻⢞⡓⢎⡑⢢⠑⡌⠢⣌⠱⣟⣾⣳⣟⡾⣽⢯⡆⢣⠘⠤⡑⢌⠒⡄⢣⠘⡄⢣⠘⡄⢳⡿⣝⣯⢻⣽⡷⡈⢆⢸⢃⢹⣿⣿⣿⣿⣿⣿⣿⣧⣴⣦⣶⣷⢉⠧⡘⢦⡉⠞⡤⢋⠴⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣯⣿⣾⣄⢣⠘⣄
⢲⣣⣿⡛⡥⢃⠎⡘⠤⡘⢄⠣⡘⠵⡠⢙⡘⢷⣻⢾⡽⣯⣟⡾⣄⠍⢢⠑⡌⠒⡌⢢⠑⡎⠐⢂⠐⢫⡽⣞⢮⣟⣾⡳⡍⢂⢾⠀⢪⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣏⠆⡬⡑⢦⡙⡜⡰⣉⠖⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡷⣟⣷⣿⡞⣬⣳⢮
⢣⣟⣷⣿⣶⡿⣾⣴⣧⠒⠆⢦⣍⣦⣑⣂⡜⣨⣛⣯⣟⡷⣯⣟⠞⢎⠄⠣⢌⠱⡈⢆⠱⡸⢤⠈⡄⣹⢞⣭⠷⣞⣷⡟⡴⣉⡎⠄⢣⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢇⠚⡰⣉⠦⡱⢌⡱⢌⡚⡔⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣯⣿⢿⡼⣯⣷
⣜⣾⢿⣳⢯⣟⣷⣻⣧⠩⣘⢯⡿⣽⣻⢿⣽⣻⡽⣾⣽⣫⠷⣉⠎⠸⡌⡱⢈⠆⡱⢈⠦⡑⢆⠱⡀⠽⠾⠭⠿⣽⣞⡿⢰⢱⡋⠔⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⡟⠌⡬⠱⢌⠲⣑⢊⡔⢣⠜⡈⠤⢈⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣾⢣⠙⠤⢉
⣼⣻⡿⣽⣻⣞⣷⣻⣧⠃⡜⣯⣟⡷⣯⣟⣾⣳⣟⡷⣯⢋⠲⡡⢎⠱⢸⡐⡡⢊⡔⢣⠜⡰⢈⠆⡱⢊⠧⡙⠶⢡⠒⡌⢣⡟⢤⣻⣿⣿⣿⣿⣿⣿⣟⣻⢋⢵⠲⣉⠖⣡⢋⡌⠳⣌⠲⠘⡁⢢⠘⠄⢊⠐⠠⢀⠉⡉⢉⠛⢿⣿⣿⣿⣿⣻⡆⣍⢢⣁
⡶⣿⣻⢷⣻⣞⣷⣻⣧⣧⣟⡷⣯⣟⣷⣻⣞⡷⣯⠟⣀⢊⡑⣈⢂⡑⢢⢱⡐⡡⣘⢂⡚⣐⢃⡚⡐⢊⠒⡐⣈⢂⡑⣈⡽⣨⣿⣿⣿⣿⣿⣿⣿⡟⢦⢣⢋⠴⡡⢌⡜⡰⠂⡌⠱⡀⢆⠑⡈⠄⡈⠐⠠⢈⠐⡀⢂⠐⠠⠈⠄⡈⢿⣿⣿⡿⡥⢆⠤⡀
⡽⣟⣯⣟⣷⣻⣞⣷⣻⢾⣭⣟⣷⣻⣞⡷⣯⡟⢣⢚⡤⠳⣌⡱⢎⡱⢣⢎⡴⢱⠌⡶⣑⢎⠶⡱⣍⢫⡍⢳⡜⣢⠳⣜⣷⣿⣿⣿⣿⣿⣿⣿⣿⡙⣎⢧⢋⠞⡱⠊⠔⡡⠁⠄⠡⡐⢂⠐⡀⠂⠄⡁⠂⠄⢂⠐⠠⠈⠄⡁⢂⠐⡀⢻⣿⣿⣿⢬⢤⡡
⣸⣿⣻⣞⣷⣻⣞⡷⣯⣟⡾⣽⣞⡷⣯⣟⠷⣉⠖⣣⠜⡳⢤⡙⣬⢃⡏⢶⡘⡆⢹⡒⠭⢎⣣⠳⣌⢣⡚⢥⢎⡱⣹⣾⣿⣿⣿⣿⣿⣿⣿⡿⢣⡑⢎⠦⣉⠎⠡⢈⠐⠠⠁⠌⡀⠇⡀⠂⠄⡁⠂⠄⠡⠈⠄⡈⠄⡁⢂⠐⡀⢂⠐⠨⣿⣿⣿⡞⢦⡹
⢾⣻⣷⣻⣞⡷⣯⣟⡷⣯⣟⡷⣯⣟⡷⢩⡒⣍⢚⡤⢋⡴⢣⡱⢆⡫⡜⢦⡹⣼⠀⣏⢧⡹⣐⢫⠜⢦⡙⡖⢪⢵⣿⣿⣿⣿⣿⣿⣿⣿⠏⡔⢣⡙⢌⠲⢁⠊⡐⠠⠈⠄⡁⠂⠄⠱⡀⢁⠂⢄⠡⠈⠄⡁⢂⠐⠠⠐⡀⢂⠐⠠⢈⠐⣿⣿⣿⣿⢣⡜
⣏⢿⣞⡷⣯⣟⡷⣯⣟⡷⣯⣟⣷⠫⣔⢣⠕⣎⠲⣼⣱⣜⣣⣕⣫⡖⣭⢃⡔⣠⢆⣈⠁⠓⡙⠮⣚⣥⠳⣜⢣⣿⣿⣿⣿⣿⣿⣿⠟⡡⠚⠌⠡⠈⠄⡂⠄⠂⠄⠡⢈⠐⡈⠔⡈⠄⢣⠀⠌⢢⠀⡁⢂⠐⠠⠈⠄⠡⠐⠠⢈⠐⠠⢈⣿⣿⣿⣯⡗⡸
⣟⣮⢻⣽⣳⢯⣟⡷⣯⣟⣷⣻⣇⠳⣌⠮⡜⣌⠳⣽⣻⣞⣯⣟⣷⢻⣽⣻⣞⣷⠋⠄⢋⠑⠒⠔⠤⣈⠙⡙⠯⢿⣿⣿⣿⣿⠟⡁⠊⠄⡉⠄⠡⢈⠢⠁⠌⡑⢈⠐⡀⢂⠐⠠⡐⠈⠄⡙⢌⣳⠀⡐⠠⠈⠄⠡⠈⠄⡁⠂⠄⡈⠐⢨⣿⣿⣿⣯⣗⡱
⡿⣮⢷⣭⣻⣟⡾⣽⣳⣿⣾⣿⡇⣹⠰⣓⠼⣌⠳⡽⢶⡹⢾⣿⣿⣿⣿⣷⣿⣾⣏⠾⡴⣌⡔⣈⠐⡀⢂⠐⡀⠂⠌⢻⡿⢁⠂⠄⡁⢂⠐⡈⠐⡀⢂⠁⠆⡐⢀⠂⡐⠠⢈⠐⠠⢁⠂⡐⠌⠒⠤⣀⠡⠈⠄⠡⢈⠐⠠⢁⠂⠄⡁⢺⣿⣿⣿⣷⣯⢶
⣹⢯⣟⣾⣳⢯⣟⡷⣯⢷⣿⣿⡇⢲⡙⣤⠛⡴⢩⡟⣭⢻⣽⡿⣿⣻⢿⣟⣯⡟⠠⠉⠌⡉⠑⠠⢁⠐⡀⠂⠄⠡⠈⠄⡐⢄⠈⡐⢀⢂⠐⠠⢁⠐⠠⢈⠐⠠⢂⠐⠠⢁⠂⠌⡐⢀⢂⠜⠠⠁⠆⠠⢉⠒⢌⡐⠠⢈⠐⠠⢈⠐⡀⢿⣿⣿⣿⣷⣿⣻
⢮⣛⢾⣳⣯⢿⡽⣿⣿⣿⢾⣿⡇⢇⡳⢔⡫⡜⢥⣻⡜⣣⢾⣿⣳⢿⣻⣯⣿⢿⣥⢫⣖⣤⠃⠆⠄⠂⠄⠡⠈⠄⡁⢂⠐⡈⢆⠐⠠⠈⠄⡁⠂⠌⡐⠠⢈⠐⠠⢈⠐⠠⢈⠐⡀⢂⡜⠠⢁⠘⡄⢁⠂⠌⠠⢈⠑⢂⠊⠴⠠⢂⠐⢻⣿⣿⣿⣿⣿⣿
⣏⢯⢳⡝⣾⣯⢿⣽⣿⡿⣯⣿⡇⢮⠱⢎⠵⢪⢱⡳⡜⡥⢿⣯⣿⣿⣯⡷⣿⣻⣷⠃⠌⠠⢉⠐⡈⠐⡈⠄⡁⢂⠐⡀⢂⠐⡈⢸⣗⡃⢆⠠⡁⠆⠠⢁⠂⡌⠐⠠⢈⠐⡀⢂⠰⢣⠜⡁⢢⠑⠄⠂⠌⠠⢁⠂⠌⠠⠈⠄⡁⠂⢌⣾⣿⣿⣿⣿⣿⣾
⣜⣎⢧⣞⣽⣞⡟⣾⣽⣻⣽⣿⡎⢲⡙⣬⠓⣭⠲⡽⣌⢳⣻⣟⣾⡿⣯⣟⡷⣿⡏⠜⣜⠳⣎⠶⣠⢃⡐⠠⠐⡀⢂⠐⡀⢂⠐⢸⠼⡍⠊⢖⡰⢌⡒⢌⡒⠤⢁⠂⠄⢂⠰⣈⢣⢃⠎⡠⢃⠌⠠⠁⠌⡐⠠⠈⠄⠡⢈⠐⠠⢁⣾⣿⣿⣿⣿⣿⣯⣟
⡷⣞⡷⣾⣻⢎⣿⣳⢿⣻⣾⣿⡇⡣⢕⢆⡛⡔⢫⠷⣌⠲⣽⣞⣷⣿⣳⣯⣟⡷⣯⡘⢤⠫⣔⢣⡕⢫⠔⣣⡑⣄⢂⠐⡀⠂⠌⣼⣏⠃⠌⠠⠙⡎⠸⢆⡹⢀⠣⣈⠰⣌⠲⡡⢎⡜⢠⡑⠠⠈⠄⡁⠂⠄⠡⠈⠄⡁⠂⠌⡐⣸⣿⣿⣿⣟⣿⣿⡷⣯
⢿⣽⣻⢷⣯⣟⡾⣽⢯⣿⣿⣿⡇⡹⡌⠮⢴⢩⢣⡟⡤⢛⣼⣟⡾⣿⣟⣷⢯⣿⡽⣟⣧⡳⢌⠣⡜⣡⠚⡤⢓⡌⣗⠺⣤⣣⣜⡷⡎⠐⡈⠄⣑⠂⡁⢂⠙⠪⡱⣌⠳⣌⢣⡑⢎⡜⠢⠐⠡⢈⠐⠠⠁⠌⠠⢁⠂⠄⡁⠂⢴⣿⣿⣿⢷⣻⣿⣿⣟⢷
⠳⡞⢿⢿⡾⣽⣻⣽⣻⢿⣿⣿⡇⢵⢊⡝⣆⢋⠶⣹⢆⡙⣾⣯⣟⣷⣻⣞⣯⡷⣿⣻⢾⡝⣯⢖⡱⢢⡙⡔⢣⠜⣬⢓⠦⣿⣿⣳⢉⢆⡐⢸⡰⢀⠐⠠⢈⠐⠠⢈⠑⠌⠓⡘⠃⠜⡡⢉⡐⠠⠈⠄⠡⢈⠐⠠⢈⠐⠠⣉⠞⠛⠿⣿⢯⣷⣻⡽⣮⣳
⠦⣘⡜⣮⣿⣳⢯⡶⣿⡿⣿⣿⡝⡢⢏⡴⢪⢍⠞⣵⢊⠴⣹⣿⢾⣳⣟⣾⣳⣟⣷⣻⢷⣻⣞⣯⡗⢣⠜⣌⢣⢚⡴⣋⢾⣿⣷⢯⡜⢢⡑⢣⠇⠠⢈⠐⠠⢈⠐⠠⠈⠄⡑⠤⣁⠂⠄⡑⠌⡓⡌⠠⢁⠂⠌⡐⠠⠈⢄⢃⠺⣟⢶⣌⢻⣯⣷⣿⣷⢯
⣿⣳⢿⣯⣷⣯⡿⣽⢶⣻⢿⣿⡧⣙⢎⡴⢋⡜⢮⡝⡎⡜⣹⡿⢯⣷⢯⣟⣿⡾⣯⣟⣯⢷⣻⡽⢌⡣⠞⡤⢃⠎⢶⡙⣮⠙⣎⢣⠚⣥⢊⠵⡈⠐⠠⢈⠐⠠⠈⠄⡁⢂⠐⠠⢀⠣⠐⡀⢂⡐⢡⠂⠄⡈⠐⠠⠁⠌⡜⣼⡆⢹⡆⢿⣆⢹⣿⡿⣝⡫
⣫⣽⡿⣾⣟⣾⣿⡿⣿⣯⣟⣿⡧⢍⠶⣘⢇⡺⣡⢟⡴⣉⢿⣽⣻⡾⣟⡿⣿⣿⣳⣯⣟⡿⢯⠓⣌⢳⡹⡰⣉⠞⡦⣙⠶⣉⢆⢣⡙⢤⢋⠞⠠⢁⠂⠄⡈⠄⡁⢂⠐⠠⢈⠐⡀⢂⠡⠐⠠⠘⠤⡉⢆⠠⢁⠂⡁⡾⢡⣿⣏⡄⢣⢚⣿⡄⣿⣟⣧⢿
⣽⣷⣻⢧⣿⣻⡽⣷⣣⣿⣞⣿⣇⢫⡜⡱⢎⡵⢊⡿⡰⢩⢾⡿⣽⡿⣽⡽⣳⢯⣿⡽⣿⡞⡅⢣⢌⢣⢳⡱⣌⠚⡴⣉⠾⣑⠎⢦⡙⢦⡉⢎⠐⠠⢈⠐⡀⢂⠐⠠⢈⠐⡀⢂⠐⡀⢂⠁⢂⠁⢢⠑⠌⠒⡀⢂⣼⠃⣿⢼⡷⢸⡀⢎⡿⠄⣻⣟⡾⣟
⣿⢾⣯⣟⣾⣽⣿⣳⣟⡾⣽⣾⡇⣇⢚⠵⣩⠖⡭⣞⠥⣋⢾⣟⡿⣽⣷⣻⣽⣻⣾⣟⣿⣳⠈⠆⡌⢂⠣⢱⣎⠓⡰⢡⢏⡇⡚⢤⡙⠦⡙⢆⠈⡐⢀⠂⡐⠠⢈⠐⡀⢂⠐⡀⢂⠐⠠⠈⠄⡈⠄⠩⢀⠡⠐⣰⣿⠀⣿⢺⣿⠰⡇⠌⡟⢰⡿⣽⣻⡽
⣿⣻⢷⣻⢯⡿⣟⡿⣿⣿⣿⣿⡗⡬⢍⡞⣡⢞⡱⣏⡖⣡⢿⣻⡼⢧⣛⢿⣻⢿⣽⣾⢯⣿⣷⣌⡰⢁⠎⡔⢎⠰⢡⢋⢾⠰⣉⠦⡙⢆⠱⣂⠐⡀⢂⠐⠠⢁⠂⡐⢀⠂⡐⢀⠂⠌⠠⢁⠂⡐⢈⠐⡀⢂⢡⣿⣿⣧⠘⢯⣿⡐⣏⠰⢰⡿⣹⣗⢯⣟
⣿⣻⢯⡿⣿⡽⣞⡽⣯⣿⢿⣿⡧⢱⢎⡼⠱⢎⡱⣏⠖⣡⢾⡿⣽⣏⡟⡾⢭⣛⣾⣻⣿⣾⣳⣟⡿⣷⠎⡜⢢⢁⢃⠎⣇⠳⣌⠲⣉⢆⠳⠌⡆⠐⠠⢈⠐⡀⢂⠐⡀⢂⠐⠠⠈⠄⡁⢂⠐⡀⢂⡐⢰⠼⣿⢿⣿⡿⣟⣦⢤⡥⣔⢻⡄⣿⡳⢎⡳⣮
⣾⡽⣯⣟⣷⣻⣽⣻⣽⣯⣿⣿⣇⠓⡮⢜⡹⢬⠱⣏⡳⢌⢾⡿⣷⢯⡿⣽⣳⡽⡶⢯⣻⢿⣿⣯⡟⢞⡱⢌⠡⢎⠢⣙⢆⠳⣌⠳⡌⠦⡉⠖⣨⢃⠐⡀⢂⠐⡀⢂⠐⠠⠈⠄⡁⢂⠐⡀⢂⠄⣣⢜⠣⠒⡈⢿⣟⣿⣻⣽⣎⣷⡩⣾⠆⣸⣿⣭⡷⣟
⠥⣫⢕⡛⡞⡽⢳⠟⡷⢯⢾⣿⡧⢹⡘⣥⢋⠶⣙⢮⡓⡜⣺⢿⣽⣻⡽⣷⢯⣷⣟⣯⣟⣯⢿⣽⣚⠆⡘⢄⠣⢌⠢⡍⢎⡱⢌⠳⣌⢣⡙⣌⠲⡌⢆⠐⡀⢂⠐⠠⠈⠄⡁⢂⠰⡀⢎⠴⡡⢎⡱⢎⡑⠠⠐⡈⢿⣿⣟⡷⣯⢞⣷⣹⠇⣼⣷⣯⢿⣽
⠴⣡⢎⡴⣡⠜⣡⢋⡜⣡⠛⠞⢧⠣⡝⡆⢏⡎⡱⢏⡳⡘⢽⠻⢾⢳⢿⣽⣻⢷⣻⣟⣾⡿⣯⣿⢷⣮⣴⣎⣔⣈⠖⡩⢆⡱⢊⠵⣈⠦⡱⢌⠳⡌⡘⡄⠐⠠⠈⠤⣑⢤⠱⣌⠲⡙⣌⠲⡑⢎⠔⡡⠄⡁⢂⠐⡈⢿⣾⣽⣳⢯⣷⢯⡀⣿⣳⣯⢿⣽
⡜⣡⠞⣰⠣⣝⢢⠳⣘⢆⠻⡜⢦⡛⡼⣘⢣⡜⣱⡉⢖⡩⢆⠫⡜⣌⠒⠦⡙⢎⠵⣩⢳⠻⣝⣫⢟⡻⣟⠿⡷⡌⢧⡑⢎⠴⣉⠖⡡⢎⡱⢊⠵⣘⠴⢁⡌⠴⣉⠖⡡⢎⡱⢌⢣⠱⣌⠣⠁⠌⠂⠄⢣⠐⡀⢂⠐⡈⢳⣯⣟⡿⣞⡿⣽⣻⡽⣯⢿⣹
**/