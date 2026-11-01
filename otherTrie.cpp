class Trie {
    vector<array<int, 26>> nxt{{}};   // node 0 = root, child 0 = "none"
    vector<bool> isEnd{false};

    int walk(const string& s) {
        int u = 0;
        for (char c : s)
            if (!(u = nxt[u][c - 'a'])) return -1;
        return u;
    }

public:
    void insert(const string& s) {
        int u = 0;
        for (char c : s) {
            if (!nxt[u][c - 'a']) {
                nxt[u][c - 'a'] = nxt.size();
                nxt.push_back({});
                isEnd.push_back(false);
            }
            u = nxt[u][c - 'a'];
        }
        isEnd[u] = true;
    }

    bool search(const string& s)      { int u = walk(s); return u != -1 && isEnd[u]; }
    bool startsWith(const string& p)  { return walk(p) != -1; }
};
