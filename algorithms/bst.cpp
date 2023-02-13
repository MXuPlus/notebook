#include <bits/stdc++.h>

/*
 * This file is about normal BST
 * contains normal method such as traverse and insert
 * Written by MichaelXu(mxuplus@outlook.com, https://github.com/MXuPlus)
 *
 * */

using namespace std;

// Definition to a node
// Acceptable type: int, float, char...
template<typename T>
struct TreeNode {
    T data;
    TreeNode<T> *left, *right, *p;
};

// Definition to a BST
// Acceptable type: int, float, char...
template<typename T>
class BSTree{
private:
    void move_subtree(TreeNode<T> *_dest, TreeNode<T> *_src);
    void layer_order_traverse();
    void pre_order_traverse(TreeNode<T> &_node);
    void in_order_traverse();
    void post_order_traverse(TreeNode<T> &_node);
    TreeNode<T> *root = nullptr;
public:
    void insert(T _data);
    void traverse(int _ttype = 0);
    TreeNode<T> *min_val(TreeNode<T> *_node);
    TreeNode<T> *max_val(TreeNode<T> *_node);
    TreeNode<T> *successor(TreeNode<T> *_node);
    TreeNode<T> *predecessor(TreeNode<T> *_node);
    void remove(TreeNode<T> *_node);
    TreeNode<T> *search(T _data);
    char *serialize();
    size_t count = 0;
};

// copy subtree, from _src to _dest
// notice that this function cannot move subtree from _src to _dest
// THIS FUNC IS FOR REMOVE_CHILD_PROCESS ONLY
template<typename T>
void BSTree<T>::move_subtree(TreeNode<T> *_dest, TreeNode<T> *_src) {
    TreeNode<T> *dp = _dest->p;
    if (dp == nullptr) root = _src;
    else if (_dest == dp->left) dp->left = _src;
    else dp->right = _src;
    if (_src) _src->p = dp;
}

// Layer-order traversal, using queue in STL.
// You can write a queue yourself in C language
template<typename T>
void BSTree<T>::layer_order_traverse() {
    if (root == nullptr) return;
    queue<TreeNode<T> *> node_queue;
    node_queue.push(root);
    while (!node_queue.empty()) {
        TreeNode<T> *p = node_queue.front();
        cout << p->data << " ";
        node_queue.pop();
        if (p->left) node_queue.push(p->left);
        if (p->right) node_queue.push(p->right);
    }
}

// Recursive Pre-order traversal
template<typename T>
void BSTree<T>::pre_order_traverse(TreeNode<T> &_node) {
    cout << _node.data << " ";
    if (_node.left) pre_order_traverse(*_node.left);
    if (_node.right) pre_order_traverse(*_node.right);
}

// In order traversal, use non-recursive method
// Using stack in STL, you can also write a stack in C language yourself
// Normally, stack uses static memory space
template<typename T>
void BSTree<T>::in_order_traverse() {
    stack <TreeNode<T> *> node_stack;
    TreeNode<T> *p = root;
    while (p != nullptr || !node_stack.empty()) {
        if (p) {
            node_stack.push(p);
            p = p->left;
        } else {
            p = node_stack.top();
            cout << p->data << " ";
            node_stack.pop();
            p = p->right;
        }
    }
}

// Recursive Post-Order traversal
template<typename T>
void BSTree<T>::post_order_traverse(TreeNode<T> &_node) {
    if (_node.left) post_order_traverse(*_node.left);
    if (_node.right) post_order_traverse(*_node.right);
    cout << _node.data << " ";
}

// Insert a node into BST
// todo: two node with same value
//       I plan to do this in Red-Black-Tree or AVL-Tree
//       those two will work in "set" data structure
template<typename T>
void BSTree<T>::insert(T _data) {
    TreeNode<T> *pt = root;
    TreeNode<T> *parent = nullptr;
    auto new_node = new TreeNode<T>{_data, nullptr, nullptr, nullptr};
    while (pt) {
        parent = pt;
        pt = _data < pt->data ? pt->left : pt->right;
    }
    new_node->p = parent;
    if (parent == nullptr) root = new_node;
    else _data < parent->data ? parent->left = new_node : parent->right = new_node;
    count ++;
}

// traversal type:
#define IN_ORD 0
#define PRE_ORD 1
#define POST_ORD 2
#define LAY_ORD 3

// Traversal function, param: traversal type
template<typename T>
void BSTree<T>::traverse(int type) {
    switch (type) {
        case PRE_ORD: cout << "Pre-order Traversal: ";pre_order_traverse(*root);break;
        case POST_ORD: cout << "Post-order Traversal: ";post_order_traverse(*root);break;
        case LAY_ORD: cout << "Lay-order Traversal: ";layer_order_traverse();break;
        default: cout << "In-order Traversal: ";in_order_traverse();
    }
    cout << endl;
}

// find the minval of the subtree
template<typename T>
TreeNode<T> *BSTree<T>::min_val(TreeNode<T> *_node) {
    TreeNode<T> *p = _node;
    while (p->left) {
        p = p->left;
    }
    return p;
}

// find the maxval of the subtree
template<typename T>
TreeNode<T> *BSTree<T>::max_val(TreeNode<T> *_node) {
    TreeNode<T> *p = root;
    while (p->right) {
        p = p->right;
    }
    return p;
}

// In-Order Traversal successor node
template<typename T>
TreeNode<T> *BSTree<T>::successor(TreeNode<T> *_node) {
    if (_node->right) return min_val(_node->right);
    TreeNode<T> *p = _node->p;
    while (p && _node == p->right) {
        _node = p; p = p->p;
    }
    return p;
}

template<typename T>
TreeNode<T> *BSTree<T>::predecessor(TreeNode<T> *_node) {
    if (_node->left) return max_val(_node->left);
    TreeNode<T> *p = _node->p;
    while (p && _node == p->left) {
        _node = p; p = p->p;
    }
    return p;
}

// remove a node, remove method is complex...
template<typename T>
void BSTree<T>::remove(TreeNode<T> *_node) {
    if (_node == nullptr) return;
    if (!(_node->left && _node->right)) {
        auto p = (TreeNode<T> *)((uintptr_t)_node->left|(uintptr_t)_node->right);
        move_subtree(_node, p);
    } else {
        // in this condition, successor of _node is min_val(_node->right)
        TreeNode<T> *p = min_val(_node->right);
        if (p != _node->right) {
            move_subtree(p, p->right);
            p->right = _node->right;
            p->right->p = p;
        }
        move_subtree(_node, p);
        p->left = _node->left;
        p->left->p = p;
    }
    delete _node; count --;
}

// Search the node whose data equals to _data
// retval: return the pointer to the node
// if no node was found, retval is nullptr
// if there are so many nodes acceptable, retval will be the oldest node
template<typename T>
TreeNode<T> *BSTree<T>::search(T _data) {
    TreeNode<T> *p = root;
    while (p && p->data != _data) {
        if (_data < p->data) p = p->left;
        else p = p->right;
    }
    return p;
}

// todo: Serialize function (translate a BST into string)
//       same as insert function, i also plan to do that in AVL-Tree and Red-black-Tree.
template<typename T>
char *BSTree<T>::serialize() {
    return nullptr;
}

int main() {
    BSTree<int> tree;
    for (int i = 0; i < 20; ++i) {
        tree.insert(i * 1314 % 59); // little rand generator
    }
    // 0 4 5 9 10 15 16 20 21 26 31 32 36 37 42 47 48 52 53 58
    cout << tree.count << endl;
    tree.traverse();
    tree.remove(tree.search(3)); // non-exist value test
    tree.remove(tree.search(0)); // test
    tree.remove(tree.search(31)); // test
    tree.remove(tree.search(16)); // root value test
    cout << tree.count << endl; // count test
    tree.traverse(POST_ORD);
    return 0;
}