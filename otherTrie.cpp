#include <array>
#include <string>
#include <vector>
using namespace std;

// Array backed trie
class Trie {
    // One row per node. The initializer {{}} creates a single zero-filled
    // row: that's the root (node 0) with no children yet.
    vector<array<int, 26>> nxt{{}};

    // Parallel to nxt: isEnd[u] says whether a word terminates at node u.
    // Starts with one entry for the root.
    vector<bool> isEnd{false};

    // Follow the characters of s from the root.
    // Returns the index of the node where s ends, or -1 if some edge
    // along the way doesn't exist (i.e. no inserted word has s as a prefix).
    int walk(const string& s) const {
        int u = 0;                              // start at the root
        for (char c : s) {
            // Move to the child for c. The assignment's value is the new u,
            // so if it's 0 the edge was missing and we bail out.
            //   equivalent to:  u = nxt[u][c - 'a']; if (u == 0) return -1;
            if (!(u = nxt[u][c - 'a'])) {
                return -1;
            }
        }
        return u;                               // node reached after the last char
    }

public:
    Trie() {}

    // Add word to the trie, creating any nodes that don't exist yet.
    void insert(const string& word) {
        int u = 0;                              // start at the root
        for (char c : word) {
            int ci = c - 'a';                   // letter -> 0..25

            if (!nxt[u][ci]) {                  // no child for this letter yet
                // The new node's index is the current number of nodes,
                // since push_back will place it at the end.
                nxt[u][ci] = nxt.size();

                // Actually create the node: an empty child row and an
                // isEnd flag, keeping the two vectors the same length.
                nxt.push_back({});
                isEnd.push_back(false);
            }

            u = nxt[u][ci];                     // step down to the child
        }
        isEnd[u] = true;                        // mark the last node as a word end
    }

    // True only if word was inserted exactly (not just as a prefix).
    // e.g. after inserting "cat": search("cat") = true, search("ca") = false.
    bool search(const string& word) const {
        int u = walk(word);
        return u != -1 && isEnd[u];             // path exists AND a word ends here
    }

    // True if any inserted word begins with prefix.
    // e.g. after inserting "cat": startsWith("ca") = true.
    bool startsWith(const string& prefix) const {
        return walk(prefix) != -1;              // the path existing is enough
    }
};
