// note that this leaks memory if no destructor written + poor cache locality

class Trie {
    struct Node {
        Node* next[26] = {};
        int wordCount = 0, prefixCount = 0;
    };
    Node* root = new Node();

public:
    void insert(const string& s) {
        Node* cur = root;
        for (char c : s) {
            if (!cur->next[c - 'a']) cur->next[c - 'a'] = new Node();
            cur = cur->next[c - 'a'];
            cur->prefixCount++;
        }
        cur->wordCount++;
    }

    Node* walk(const string& s) {
        Node* cur = root;
        for (char c : s) {
            cur = cur->next[c - 'a'];
            if (!cur) return nullptr;
        }
        return cur;
    }

    bool contains(const string& s)  { Node* n = walk(s); return n && n->wordCount > 0; }
    int countPrefix(const string& p){ Node* n = walk(p); return n ? n->prefixCount : 0; }
};
