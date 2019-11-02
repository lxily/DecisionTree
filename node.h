#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

struct TreeNode{
    string feature;
    string label;
    bool isLeaf;
    vector<TreeNode*>child;
    TreeNode(){isLeaf=false;}
};

#endif // NODE_H_INCLUDED
