#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct _node {
    int key;
    struct _node *left, *right, *parent;
};

struct _tree {
    struct _node* root;
};

typedef struct _node Node;
typedef struct _tree Tree;

int tree_search(Tree* tree, int key, Node** result);
void tree_insert(Tree* tree, int key);
void tree_delete(Tree* tree, int key);
void __tree_transplant(Tree* tree, Node* u, Node* v);
Node* node_minimum(Node* node);
Node* node_maximum(Node* node);
Node* tree_successor(Node* node);
Node* tree_predecessor(Node* node);
void tree_inorder(Tree* tree);
void tree_preorder(Tree* tree);
void tree_postorder(Tree* tree);
void tree_inorder_rec(Node* node);
void tree_preorder_rec(Node* node);
void tree_postorder_rec(Node* node);
void tree_postorder_morris(Tree* tree);

int
tree_search(Tree* tree, int key, Node** result) {
    Node *root = tree->root, *parent = NULL;
    while (root != NULL && key != root->key) {
        parent = root;
        if (key < root->key) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    if (root == NULL) {
        *result = parent;
        return 1;
    }
    *result = root;
    return 0;
}

void
tree_insert(Tree* tree, int key) {
    Node* parent = NULL;
    if (tree_search(tree, key, &parent) == 0) {
        return;
    }
    Node* node = malloc(sizeof(Node));
    node->key = key;
    if (parent == NULL) {
        tree->root = node;
    } else if (key < parent->key) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    node->parent = parent;
}

void
__tree_transplant(Tree* tree, Node* u, Node* v) {
    if (u->parent == NULL) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != NULL) {
        v->parent = u->parent;
    }
}

Node*
node_minimum(Node* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

Node*
tree_successor(Node* node) {
    if (node->right != NULL) {
        return node_minimum(node->right);
    }
    Node* successor = node->parent;
    while (successor != NULL && node == successor->right) {
        node = successor;
        successor = successor->parent;
    }
    return successor;
}

Node*
node_maximum(Node* node) {
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

Node*
tree_predecessor(Node* node) {
    if (node->left != NULL) {
        return node_maximum(node->left);
    }
    Node* predecessor = node->parent;
    while (predecessor != NULL && node == predecessor->left) {
        node = predecessor;
        predecessor = predecessor->parent;
    }
    return predecessor;
}

void
tree_delete(Tree* tree, int key) {
    Node *successor = NULL, *node = NULL;
    if (tree_search(tree, key, &node) == 1) {
        return;
    }
    if (node->left == NULL) {
        __tree_transplant(tree, node, node->right);
    } else if (node->right == NULL) {
        __tree_transplant(tree, node, node->left);
    } else {
        successor = node_minimum(node->right);
        if (successor != node->right) {
            __tree_transplant(tree, successor, successor->right);
            successor->right = node->right;
            successor->right->parent = successor;
        }
        __tree_transplant(tree, node, successor);
        successor->left = node->left;
        successor->left->parent = successor;
    }
    free(node);
}

void
tree_inorder_rec(Node* node) {
    if (node == NULL) {
        return;
    }
    tree_inorder_rec(node->left);
    printf("%d ", node->key);
    tree_inorder_rec(node->right);
}

void
tree_inorder(Tree* tree) {
    tree_inorder_rec(tree->root);
}

void
tree_postorder_rec(Node* node) {
    if (node == NULL) {
        return;
    }
    tree_postorder_rec(node->left);
    tree_postorder_rec(node->right);
    printf("%d ", node->key);
}

void
tree_postorder(Tree* tree) {
    tree_postorder_rec(tree->root);
}

void
tree_preorder_rec(Node* node) {
    if (node == NULL) {
        return;
    }
    printf("%d ", node->key);
    tree_preorder_rec(node->left);
    tree_preorder_rec(node->right);
}

void
tree_preorder(Tree* tree) {
    tree_preorder_rec(tree->root);
}

void
tree_postorder_morris(Tree* tree) {
    Node* current = NULL;
    Node *previous = NULL, *predecessor = NULL;
    Node *successor = NULL, *temp = NULL;
    if (tree->root == NULL) {
        return;
    }
    current = malloc(sizeof(Node));
    current->left = tree->root;
    while (current != NULL) {
        if (current->left == NULL) {
            current = current->right;
        } else {
            predecessor = current->left;
            while (predecessor->right != NULL && predecessor->right != current) {
                predecessor = predecessor->right;
            }
            if (predecessor->right == NULL) {
                predecessor->right = current;
                current = current->left;
            } else {
                predecessor->right = NULL;
                successor = current;
                current = current->left;
                previous = NULL;
                while (current != NULL) {
                    temp = current->right;
                    current->right = previous;
                    previous = current;
                    current = temp;
                }
                while (previous != NULL) {
                    temp = previous->right;
                    previous->right = current;
                    current = previous;
                    printf("%d ", previous->key);
                    previous = temp;
                }
                current = successor;
                current = current->right;
            }
        }
    }
    free(successor);
}
