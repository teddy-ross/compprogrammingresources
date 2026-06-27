struct DSU {
    vector<int> par, sz;
    DSU(int n) : par(n), sz(n, 1) { iota(par.begin(), par.end(), 0); }
    int find(int x) { return par[x] == x ? x : par[x] = find(par[x]); }   // path compression
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);          // union by size
        par[b] = a; sz[a] += sz[b];
        return true;
    }
};

// Reuses Edge { int u, v; long long w; }. 0-indexed.
// Returns total MST weight, or -1 if disconnected. mstEdges (optional) collects chosen edges.
long long kruskal(int n, vector<Edge> edges, vector<Edge>* mstEdges = nullptr) {
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) { return a.w < b.w; });
    DSU dsu(n);
    long long total = 0;
    int used = 0;
    for (const auto& e : edges)
        if (dsu.unite(e.u, e.v)) {
            total += e.w;
            if (mstEdges) mstEdges->push_back(e);
            if (++used == n - 1) break;          // tree complete, stop early
        }
    return used == n - 1 ? total : -1;           // < n-1 edges joined => disconnected
}
