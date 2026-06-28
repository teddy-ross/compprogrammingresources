unordered_map<TreeNode*, TreeNode*> parent;
function<void(TreeNode*, TreeNode*)> build = [&](TreeNode* node, TreeNode* par) {
    if (!node) return;
    parent[node] = par;
    build(node->left, node);
    build(node->right, node);
};
build(root, nullptr);
