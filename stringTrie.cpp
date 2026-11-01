// String trie + optional Aho-Corasick.
//
//   StringTrie t(totalLen);           // reserve is optional
//   int id = t.add("abc");            // returns end node; add(s, -1) erases (s must be present)
//   t.contains("abc"); t.countPrefix("ab");
//
// Aho-Corasick: insert all patterns, then call build(). After build():
//   - nxt[u][c] is the full automaton transition (missing edges filled via failure links),
//     so DON'T insert / walk / contains / countPrefix afterwards.
//   - out[u] = number of patterns (with multiplicity) that end at u or any suffix-link ancestor.
//     "Avoid all patterns" DP: state u is bad iff out[u] > 0.
//   - totalMatches(text): total pattern occurrences in text.
//   - hitsPerNode(text)[endNode]: occurrences of the pattern ending at endNode.
#include <bits/stdc++.h>
using namespace std;

struct StringTrie {
    static constexpr int A = 26;
    static constexpr char BASE = 'a';

    vector<array<int, A>> nxt;   // 0 = no child (root is 0 and is never a child) / after build: go to root
    vector<int> pre, cnt;        // #words through node, #words ending at node
    vector<int> link, order;     // Aho-Corasick: failure link, BFS order
    vector<long long> out;
    bool built = false;

    explicit StringTrie(int reserveNodes = 0) {
        nxt.reserve(reserveNodes + 1);
        pre.reserve(reserveNodes + 1);
        cnt.reserve(reserveNodes + 1);
        newNode();
    }

    int newNode() {
        nxt.push_back({});
        pre.push_back(0);
        cnt.push_back(0);
        return (int)nxt.size() - 1;
    }

    int size() const { return (int)nxt.size(); }

    // d = +1 insert, d = -1 erase (s must be present). Returns end node.
    int add(const string& s, int d = 1) {
        assert(!built);
        int u = 0;
        pre[0] += d;
        for (char ch : s) {
            int c = ch - BASE;
            if (!nxt[u][c]) { int v = newNode(); nxt[u][c] = v; }
            u = nxt[u][c];
            pre[u] += d;
        }
        cnt[u] += d;
        return u;
    }

    int walk(const string& s) const {          // -1 if path doesn't exist
        int u = 0;
        for (char ch : s) {
            u = nxt[u][ch - BASE];
            if (!u) return -1;
        }
        return u;
    }

    bool contains(const string& s) const { int u = walk(s); return u != -1 && cnt[u] > 0; }
    int countPrefix(const string& p) const { int u = walk(p); return u == -1 ? 0 : pre[u]; }

    void clear() {
        nxt.clear(); pre.clear(); cnt.clear();
        link.clear(); order.clear(); out.clear();
        built = false;
        newNode();
    }

    // ---------------- Aho-Corasick ----------------
    void build() {
        int n = size();
        link.assign(n, 0);
        out.assign(cnt.begin(), cnt.end());
        order.clear();
        order.reserve(n);
        for (int c = 0; c < A; c++)
            if (int v = nxt[0][c]) { link[v] = 0; order.push_back(v); }
        for (size_t i = 0; i < order.size(); i++) {
            int u = order[i];
            out[u] += out[link[u]];
            for (int c = 0; c < A; c++) {
                int v = nxt[u][c];
                if (v) { link[v] = nxt[link[u]][c]; order.push_back(v); }
                else nxt[u][c] = nxt[link[u]][c];
            }
        }
        built = true;
    }

    int step(int u, char ch) const { return nxt[u][ch - BASE]; }

    long long totalMatches(const string& text) const {
        assert(built);
        long long res = 0;
        int u = 0;
        for (char ch : text) { u = nxt[u][ch - BASE]; res += out[u]; }
        return res;
    }

    // hits[v] = #times the string spelled by node v occurs in text.
    // Pattern occurrences: hits[endNodeReturnedByAdd].
    vector<long long> hitsPerNode(const string& text) const {
        assert(built);
        vector<long long> hits(size(), 0);
        int u = 0;
        for (char ch : text) { u = nxt[u][ch - BASE]; hits[u]++; }
        for (int i = (int)order.size() - 1; i >= 0; i--) {
            int v = order[i];
            hits[link[v]] += hits[v];
        }
        return hits;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

}
