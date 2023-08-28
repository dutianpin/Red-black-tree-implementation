#include <stdio.h>
#include <stdlib.h>

// 定义节点颜色
typedef enum Color {
    RED,
    BLACK
} Color;

// 红黑树的节点定义
typedef struct RBTreeNode {
    int data;                   // 数据
    Color color;                // 颜色
    struct RBTreeNode *left;    // 左子节点
    struct RBTreeNode *right;   // 右子节点
    struct RBTreeNode *parent;  // 父节点
} RBTreeNode;

// 红黑树结构定义
typedef struct RBTree {
    RBTreeNode *root;   // 根节点
} RBTree;

// 创建新的红黑树节点
RBTreeNode* create_node(int data) {
    RBTreeNode *newNode = malloc(sizeof(RBTreeNode));
    if (!newNode) {
        exit(EXIT_FAILURE);  // 如果内存分配失败，退出程序
    }
    newNode->data = data;
    newNode->color = RED;  // 新节点默认为红色
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    return newNode;
}

// 创建红黑树
RBTree* create_tree() {
    RBTree *tree = malloc(sizeof(RBTree));
    if (!tree) {
        exit(EXIT_FAILURE);
    }
    tree->root = NULL;
    return tree;
}

// 查找红黑树中的节点
RBTreeNode* search_tree(RBTreeNode *root, int data) {
    while (root && root->data != data) {
        if (data < root->data) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return root;
}

// ... 更多代码将在下一部分提供
// 左旋操作
void left_rotate(RBTree *tree, RBTreeNode *x) {
    RBTreeNode *y = x->right;  // 设置y
    x->right = y->left;  // 将y的左子树变为x的右子树
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;  // 更新y的parent
    if (x->parent == NULL) {
        tree->root = y;  // 如果x是根节点，y成为新的根节点
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;  // 将x放到y的左边
    x->parent = y;
}

// 右旋操作
void right_rotate(RBTree *tree, RBTreeNode *y) {
    RBTreeNode *x = y->left;
    y->left = x->right;
    if (x->right != NULL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NULL) {
        tree->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

// 插入新节点后进行修复
void insert_fixup(RBTree *tree, RBTreeNode *z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBTreeNode *y = z->parent->parent->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(tree, z->parent->parent);
            }
        } else {
            // 对称地处理右子节点
            RBTreeNode *y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

// 插入新节点到红黑树中
void insert_tree(RBTree *tree, int data) {
    RBTreeNode *z = create_node(data);
    RBTreeNode *y = NULL;
    RBTreeNode *x = tree->root;

    while (x) {
        y = x;
        if (data < x->data) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;
    if (!y) {
        tree->root = z;
    } else if (data < y->data) {
        y->left = z;
    } else {
        y->right = z;
    }

    insert_fixup(tree, z);
}

// ... 更多代码将在下一部分提供

// 获取最小节点
RBTreeNode* tree_minimum(RBTreeNode *node) {
    while (node->left) {
        node = node->left;
    }
    return node;
}

// 获取最大节点
RBTreeNode* tree_maximum(RBTreeNode *node) {
    while (node->right) {
        node = node->right;
    }
    return node;
}

// 删除修复
void delete_fixup(RBTree *tree, RBTreeNode *x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBTreeNode *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            RBTreeNode *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

// 从红黑树中删除节点
void delete_tree(RBTree *tree, RBTreeNode *z) {
    RBTreeNode *y;
    RBTreeNode *x;

    if (!z->left || !z->right) {
        y = z;
    } else {
        y = tree_minimum(z->right);
    }

    if (y->left) {
        x = y->left;
    } else {
        x = y->right;
    }

    x->parent = y->parent;

    if (!y->parent) {
        tree->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    if (y != z) {
        z->data = y->data;
    }

    if (y->color == BLACK) {
        delete_fixup(tree, x);
    }
    free(y);
}

// ... 更多代码将在下一部分提供

// 中序遍历红黑树
void inorder_traversal(RBTreeNode *root) {
    if (root) {
        inorder_traversal(root->left);
        printf("%d(%s) ", root->data, root->color == RED ? "RED" : "BLACK");
        inorder_traversal(root->right);
    }
}

// 释放红黑树节点内存
void free_tree(RBTreeNode *root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

// 主函数
int main() {
    RBTree *tree = create_tree();
    
    // 示例：插入若干节点
    insert_tree(tree, 45);
    insert_tree(tree, 38);
    insert_tree(tree, 58);
    insert_tree(tree, 17);
    insert_tree(tree, 31);
    insert_tree(tree, 25);
    insert_tree(tree, 20);

    printf("Inorder Traversal:\n");
    inorder_traversal(tree->root);
    printf("\n");

    // 示例：删除某个节点
    RBTreeNode *node_to_delete = tree->root->left; // Just for demonstration
    if (node_to_delete) {
        delete_tree(tree, node_to_delete);
        printf("After Deletion:\n");
        inorder_traversal(tree->root);
        printf("\n");
    }

    free_tree(tree->root);
    free(tree);

    return 0;
}

