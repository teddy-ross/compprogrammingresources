class DSU {
    vector<int> parent, size_;
    int components;

public:
    DSU(int n) : parent(n), size_(n, 1), components(n) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        if (size_[px] < size_[py]) swap(px, py);
        parent[py] = px;
        size_[px] += size_[py];
        --components;
        return true;
    }

    bool connected(int x, int y) { return find(x) == find(y); }
    int getSize(int x) { return size_[find(x)]; }
    int countComponents() { return components; }
};
