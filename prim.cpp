// Lazy Prim. adj[u] = list of (neighbor, weight); push both directions for undirected.
// 0-indexed. Returns total MST weight, or -1 if disconnected.
long long primLazy(int n, const vector<vector<pair<int,long long>>>& adj, int src = 0) {
    vector<char> inMST(n, false);
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
    pq.push({0, src});
    long long total = 0;
    int count = 0;
    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (inMST[u]) continue;                  // stale entry; same lazy deletion as Dijkstra
        inMST[u] = true;
        total += w;
        ++count;
        for (auto [v, weight] : adj[u])
            if (!inMST[v]) pq.push({weight, v});
    }
    return count == n ? total : -1;
}
