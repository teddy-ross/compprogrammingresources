// =============================================================================
//  DIJKSTRA — contest template (LeetCode / CP)
// =============================================================================
//
//  THE GENERALIZING PRINCIPLE (recognize Dijkstra when it doesn't look like it):
//    Dijkstra works for ANY relaxation that is MONOTONE — the candidate value at
//    a neighbor is always >= the value at the node you're expanding from. Then
//    "pop the smallest tentative value" is safe because nothing popped later can
//    improve what's already finalized.
//      sum            nd = d + w           (w >= 0)            -> classic shortest path
//      bottleneck-max nd = max(d, w)       (max >= d)          -> min effort / swim in water
//      bottleneck-min nd = min(d, w) w/ max-heap               -> widest path
//      product (prob)  use max-heap + multiply, or -log + min  -> max-probability path
//    If the relaxation can DECREASE below d (negative edges), Dijkstra is wrong:
//      negative weights -> Bellman-Ford / SPFA;  all-pairs small V -> Floyd-Warshall.
//
//  GOTCHAS:
//    - min-heap needs greater<>; default priority_queue is a MAX-heap.
//    - use long long + INF = 1e18, NOT INT_MAX. Sums overflow int, and INT_MAX + w
//      overflows before you can even compare. 1e18 + 1e9 still fits in ll.
//    - no visited[] array: skip stale popped entries with `if (d > dist[u]) continue;`
//      A node may sit in the heap multiple times; the first pop finalizes it.
// =============================================================================

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 1e18;

// -----------------------------------------------------------------------------
// 1. CORE — single-source, adjacency list. O(E log V).
//    adj[u] = { {v, w}, ... }.  dist[v] == INF means unreachable.
// -----------------------------------------------------------------------------
vector<ll> dijkstra(int n, int src, const vector<vector<pair<int,int>>>& adj) {
    vector<ll> dist(n, INF);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<>> pq;  // (dist, node)
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;                 // stale entry; already finalized better
        for (auto [v, w] : adj[u]) {
            if (d + w < dist[v]) {                 // d == dist[u] here, saves a load
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// -----------------------------------------------------------------------------
// 2. + PATH RECONSTRUCTION — track predecessor, walk back from target.
// -----------------------------------------------------------------------------
vector<int> dijkstraPath(int n, int src, int dst,
                         const vector<vector<pair<int,int>>>& adj) {
    vector<ll> dist(n, INF);
    vector<int> par(n, -1);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<>> pq;
    dist[src] = 0; pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (d + w < dist[v]) {
                dist[v] = d + w;
                par[v]  = u;                       // remember how we reached v
                pq.push({dist[v], v});
            }
        }
    }
    if (dist[dst] == INF) return {};               // unreachable
    vector<int> path;
    for (int v = dst; v != -1; v = par[v]) path.push_back(v);
    reverse(path.begin(), path.end());
    return path;
}

// -----------------------------------------------------------------------------
// 3. GRID — 4-directional. Swap the relaxation line per problem.
//    e.g. min path sum:   nd = d + grid[nr][nc]
//         min effort:     nd = max(d, abs(grid[nr][nc] - grid[r][c]))   // LC 1631
//         swim in water:  nd = max(d, grid[nr][nc])                     // LC 778
// -----------------------------------------------------------------------------
ll dijkstraGrid(const vector<vector<int>>& grid) {
    int R = grid.size(), C = grid[0].size();
    vector<vector<ll>> dist(R, vector<ll>(C, INF));
    priority_queue<tuple<ll,int,int>, vector<tuple<ll,int,int>>, greater<>> pq;
    dist[0][0] = 0;
    pq.push({0, 0, 0});
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    while (!pq.empty()) {
        auto [d, r, c] = pq.top(); pq.pop();
        if (d > dist[r][c]) continue;
        if (r == R-1 && c == C-1) return d;        // early exit once target finalized
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
            ll nd = d + grid[nr][nc];              // <-- change relaxation here
            if (nd < dist[nr][nc]) {
                dist[nr][nc] = nd;
                pq.push({nd, nr, nc});
            }
        }
    }
    return dist[R-1][C-1];
}

// -----------------------------------------------------------------------------
// 4. STATE-AUGMENTED — node becomes (vertex, state). dist[vertex][state].
//    The state is any finite extra dimension the cost depends on:
//      parity of edges used, fuel/budget remaining, keys held (bitmask),
//      0/1 = "discount used yet". Same algorithm, larger state space.
//    Skeleton (state = a small integer in [0, S)):
//
//      vector<vector<ll>> dist(n, vector<ll>(S, INF));
//      pq.push({0, src, startState});
//      ...
//      auto [d, u, s] = pq.top(); pq.pop();
//      if (d > dist[u][s]) continue;
//      for (each edge / action from (u,s)) {
//          auto [v, ns, w] = transition;          // new vertex, new state, cost
//          if (d + w < dist[v][ns]) { dist[v][ns] = d + w; pq.push({dist[v][ns], v, ns}); }
//      }
//      answer = min over s of dist[target][s];
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// 5. 0-1 BFS — weights in {0,1} ONLY. O(V+E), beats Dijkstra's log factor.
//    deque: 0-edge -> push_front (same layer), 1-edge -> push_back (next layer).
//    Shows up as "min obstacles to remove", "min cost with free moves" (LC 1368/2290).
// -----------------------------------------------------------------------------
vector<ll> zeroOneBFS(int n, int src, const vector<vector<pair<int,int>>>& adj) {
    vector<ll> dist(n, INF);
    deque<pair<ll,int>> dq;                        // (dist, node)
    dist[src] = 0; dq.push_back({0, src});
    while (!dq.empty()) {
        auto [d, u] = dq.front(); dq.pop_front();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u]) {               // w is 0 or 1
            if (d + w < dist[v]) {
                dist[v] = d + w;
                if (w == 0) dq.push_front({dist[v], v});
                else        dq.push_back({dist[v], v});
            }
        }
    }
    return dist;
}
