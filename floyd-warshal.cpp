// Reuses Edge and INF from above.
// 0-indexed. dist[i][j] == INF if no path. dist[i][i] < 0 => i on a negative cycle.
vector<vector<long long>> floydWarshall(int n, const vector<Edge>& edges, bool directed) {
    vector<vector<long long>> dist(n, vector<long long>(n, INF));
    for (int i = 0; i < n; ++i) dist[i][i] = 0;
    for (const auto& [u, v, w] : edges) {
        dist[u][v] = min(dist[u][v], w);          // min collapses parallel edges
        if (!directed) dist[v][u] = min(dist[v][u], w);
    }
    for (int k = 0; k < n; ++k)                    // k MUST be outermost
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (dist[i][k] != INF && dist[k][j] != INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
    return dist;
}
