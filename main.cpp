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

    void insertNonFull(int x, int y, char z, char key);    // insert to node
    void split(int index, Node* y); // y is the child of this node with order index and (y) is the node being split
    void print();   // print node
};

Node::Node(int t, bool leaf) {
    this->x = new int[2 * t - 1];   // key amount
    this->y = new int[2 * t - 1];   // key amount
    this->z = new char[2 * t - 1];  // key amount

    this->childs = new Node * [2 * t];   // child amount
    this->size = 0; // child size at start
    this->leaf = leaf;  // true if it is a leaf node, false otherwise
    this->t = t;
}

Node::~Node() {
    // delete childs
    delete[] this->x;
    delete[] this->y;
    delete[] this->z;
    if (this->leaf == false) {
        for (int i = 0; i <= this->size; i++) {
            delete this->childs[i];
        }
    }
    delete[] this->childs;
}

void Node::print() {
    // prints node in prefix order
    int index;
    for (index = 0; index < size; index++) {
        printf("(%d,%d,%c)", x[index], y[index], z[index]);
    }
    printf("\n");
    for (index = 0; index < size; index++) {
        if (leaf == false) {
            childs[index]->print();
        }
    }
    if (leaf == false) {
        childs[index]->print();
    }
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
    if (this->root != NULL) {
        delete this->root;
    }
}

void BTree::print() {
    // prints tree in prefix order
    if (root != NULL) {
        root->print();
    }
}

void BTree::insert(int x, int y, char z) {
    // insert new key to B-Tree
    Node *r = this->root;

    if (r == NULL) {
        // if root is undefined, create root and insert key
        this->root = new Node(t, true);
        this->root->x[0] = x;
        this->root->y[0] = y;
        this->root->z[0] = z;
        this->root->size = 1;
    } else {
        if (root->size == 2 * t - 1) {
            Node *s = new Node(t, false); // allocate node

            this->root = s; // make new root

            s->childs[0] = r;

            s->split(0, r);

            int i = 0;
            if ( (this->key == 'x' && s->x[0] < x ) || (this->key == 'y' && s->y[0] < y) || (this->key == 'z' && s->z[0] < z) ) {
                i++;
            }

            s->childs[i]->insertNonFull(x, y, z, this->key);

        } else {
            r->insertNonFull(x, y, z, this->key);
        }
    }
}


void Node::insertNonFull(int x, int y, char z, char key) {
    // insert if it is not full
    int i = this->size - 1;

    if (this->leaf == true) {
        while ( (i >= 0 && key == 'x' && this->x[i] > x) || (i >= 0 && key == 'y' && this->y[i] > y) || (i >= 0 && key == 'z' && this->z[i] > z) )
        {
            this->x[i + 1] = this->x[i];
            this->y[i + 1] = this->y[i];
            this->z[i + 1] = this->z[i];
            i = i - 1;
        }

        this->x[i + 1] = x;
        this->y[i + 1] = y;
        this->z[i + 1] = z;
        this->size++;
    } else {
        while ( (i >= 0 && key == 'x' && this->x[i] > x) || (i >= 0 && key == 'y' && this->y[i] > y) || (i >= 0 && key == 'z' && this->z[i] > z) ) {
            i--;
        }

        if (this->childs[i + 1]->size == 2 * t - 1) {

            split(i + 1, this->childs[i + 1]);

            if ( (key == 'x' && this->x[i + 1] < x) || (key == 'y' && this->y[i + 1] < y) || (key == 'z' && this->z[i+1] < z) ) {
                i++;
            }
        }
        this->childs[i + 1]->insertNonFull(x, y, z, key);
    }
}


void Node::split(int i, Node* y) {
    // split the child
    Node *z = new Node(y->t, y->leaf);
    z->size = this->t - 1;

    for (int j = 0; j < t - 1; j++) {
        z->x[j] = y->x[j + t];
        z->y[j] = y->y[j + t];
        z->z[j] = y->z[j + t];
    }

    if (y->leaf != true) {
        for (int j = 0; j < t; j++) {
            z->childs[j] = y->childs[j + t];
        }
    }

    y->size = t - 1;
    for (int j = this->size; j >= i + 1; j--) {
        this->childs[j + 1] = this->childs[j];
    }

    this->childs[i + 1] = z;

    for (int j = this->size - 1; j >= i; j--) {
        this->x[j + 1] = this->x[j];
        this->y[j + 1] = this->y[j];
        this->z[j + 1] = this->z[j];
    }

    this->x[i] = y->x[this->t - 1];
    this->y[i] = y->y[this->t - 1];
    this->z[i] = y->z[this->t - 1];
    this->size++;
}

int main() {
    BTree* new_tree = new BTree;

    scanf("%d", &new_tree->node_count);
    scanf("%d", &new_tree->t);
    scanf(" %c", &new_tree->key);

    int x, y;
    char z;
    for (int i = 0; i < new_tree->node_count; i++) {
        scanf("%d", &x);
        scanf("%d", &y);
        scanf(" %c", &z);
        new_tree->insert(x, y, z);
    }

    new_tree->print();

    delete new_tree;

    return 0;
}