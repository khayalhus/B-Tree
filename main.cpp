#include <iostream>

using namespace std;

struct Node {
    int* x; // a key attribute
    int* y; // a key attribute
    char* z; // a key attribute

    int t;  // degree (passed from BTree)
    int size;  // amount of active childs
    Node** childs;  // child pointer holder holder
    bool leaf; // whether Node is a leaf or not

    Node(int t, bool leaf); // constructor
    ~Node();

    void insert(int x, int y, char z, char key);    // insert to node
    void split(int index, Node* y); // split node
    void print();   // print node
};

Node::Node(int t, bool leaf) {
    this->x = new int[2 * t - 1];   // key amount
    this->y = new int[2 * t - 1];   // key amount
    this->z = new char[2 * t - 1];  // key amount

    this->childs = new Node * [2 * size];   // child amount
    this->size = 0; // child size at start
    this->leaf = leaf;  // true if it is a leaf node, false otherwise
}

Node::~Node() {
    // delete childs
}

void Node::print() {
    // prints node in prefix order
    int index;
    for (index = 0; index < size; index++) {
        if (leaf == false) {
            childs[index]->print();
        }
        cout << "(" << x[index] << "," << y[index] << "," << z[index] << ")";
    }
    if (leaf == false)
        childs[index]->print();
}

struct BTree {
    int node_count; // total node count
    int t; // degree of the tree
    char key; // key attribute (x, y or z)
    Node* root; // holds the top-most node

    BTree();
    ~BTree();

    void insert(int x, int y, char z);  // inserts new key to B-Tree
    void print();   // prints tree
};

BTree::BTree() {
    root = NULL;
}

BTree::~BTree() {
    // traverse tree and free nodes
}

void BTree::print() {
    // prints tree in prefix order
    if (root != NULL) {
        root->print();
    }
}

void BTree::insert(int x, int y, char z) {
    // insert new key to B-Tree
}


void Node::insert(int x, int y, char z, char key) {
    // insert if it is not full
}


void Node::split(int i, Node* new_node) {
    // split the child
}

int main() {
    BTree* new_tree = new BTree;
    
    scanf("%d", &new_tree->node_count);
    scanf("%d", &new_tree->t);
    scanf(" %c", &new_tree->key);
    printf("!%d!%d!%c!\n", new_tree->node_count, new_tree->t, new_tree->key);

    int x, y;
    char z;
    for (int i = 0; i < new_tree->node_count; i++) {
        scanf("%d", &x);
        scanf("%d", &y);
        scanf(" %c", &z);
        printf("!%d!%d!%c!\n", x, y, z);
        //new_tree->insert(x, y, z);
    }

    //new_tree->print();

    delete new_tree;

    return 0;
}