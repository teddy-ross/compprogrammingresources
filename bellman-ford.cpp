struct Edge { int u, v; long long w; };
const long long INF = 1e18;

// 0-indexed nodes 0..n-1. dist[i] == INF if unreachable.
// hasNegCycle set if a reachable edge still relaxes after n-1 passes.
vector<long long> bellmanFord(int n, const vector<Edge>& edges, int src, bool& hasNegCycle) {
    vector<long long> dist(n, INF);
    dist[src] = 0;
    for (int pass = 0; pass < n - 1; ++pass) {
        bool changed = false;
        for (const auto& [u, v, w] : edges)
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                changed = true;
            }
        if (!changed) break;              // converged early, common case
    }
    hasNegCycle = false;
    for (const auto& [u, v, w] : edges)
        if (dist[u] != INF && dist[u] + w < dist[v]) { hasNegCycle = true; break; }
    return dist;
}
