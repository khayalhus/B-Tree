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
    bool remove(int x, int y, char z, char key);
    void merge(int left_index, int right_index);
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

bool Node::remove(int x, int y, char z, char key) {
    int index = 0;

    while ( (key == 'x' && index < this->size && this->x[index] < x) || (key == 'y' && index < this->size && this->y[index] < y) || (key == 'z' && index < this->size && this->z[index] < z) ) {
        index++;
    }

    if (index > this->size) {
        if (this->leaf == true) {
            return false;
        } else {
            return this->childs[index - 1]->remove(x, y, z, key);
        }
    }

    if ( (key == 'x' && this->x[index] == x ) || (key == 'y' && this->y[index] == y ) || (key == 'z' && this->z[index] == z ) ) {
        if (this->leaf == true) {
            // Case 1: key k is in a leaf node
            for (int i = index + 1; i < this->size; i++) {
                this->x[i - 1] = this->x[i];
                this->y[i - 1] = this->y[i];
                this->z[i - 1] = this->z[i];
            }
            this->size--;
        }
        else {
            // Case 2: key k is in an internal node

            if (this->childs[index]->size > t - 1) {
                // - Subcase a: having a child with at least t keys preceding k
                Node *traverse = this->childs[index];

                while (traverse->leaf == false) {
                    traverse = traverse->childs[traverse->size];
                }

                this->x[index] = traverse->x[traverse->size - 1];
                this->y[index] = traverse->y[traverse->size - 1];
                this->z[index] = traverse->z[traverse->size - 1];

                this->childs[index]->remove(traverse->x[traverse->size - 1], traverse->y[traverse->size - 1], traverse->z[traverse->size - 1], key);

            } else if (this->childs[index + 1]->size > t - 1) {
                // – Subcase b: having a child with at least t keys following k
                Node *traverse = this->childs[index + 1];
                
                while (traverse->leaf == false) {
                    traverse = traverse->childs[0];
                }

                this->x[index] = traverse->x[0];
                this->y[index] = traverse->y[0];
                this->z[index] = traverse->z[0];

                this->childs[index + 1]->remove(traverse->x[0], traverse->y[0], traverse->z[0], key);

            } else {
                // – Subcase c: both have t-1 keys
                this->merge(index, index + 1);
                this->childs[index]->remove(this->x[index], this->y[index], this->z[index], key);
            }
        }

        return true;

    } else if (this->leaf == false) {

        if (this->childs[index]->size == t - 1) {
            // Case 3: key k is not in an internal node and root of an appropriate subtree has only t-1 keys
            if (index != 0 && this->childs[index - 1]->size >= t) {
                // - Subcase a1: subtree has only t-1 keys having a left sibling with at least t keys
                Node *child = this->childs[index];
                Node *left_sibling = this->childs[index - 1];

                for (int i = child->size - 1; i >= 0; i--) {
                    child->x[i + 1] = child->x[i];
                    child->y[i + 1] = child->y[i];
                    child->z[i + 1] = child->z[i];
                }

                if (child->leaf == false) {
                    for (int i = child->size; i >= 0; i--) {
                        child->childs[i + 1] = child->childs[i];
                    }
                }

                child->x[0] = this->x[index - 1];
                child->y[0] = this->y[index - 1];
                child->z[0] = this->z[index - 1];

                if (child->leaf == false) {
                    child->childs[0] = left_sibling->childs[left_sibling->size];
                }

                this->x[index - 1] = left_sibling->x[left_sibling->size - 1];
                this->y[index - 1] = left_sibling->y[left_sibling->size - 1];
                this->z[index - 1] = left_sibling->z[left_sibling->size - 1];

                child->size++;
                left_sibling->size--;
            }
            else if (index != this->size && this->childs[index + 1]->size >= t) {
                // - Subcase a2: subtree has only t-1 keys having a right sibling with at least t keys
                Node *child = this->childs[index];
                Node *right_sibling = this->childs[index + 1];

                child->x[child->size] = this->x[index];
                child->y[child->size] = this->y[index];
                child->z[child->size] = this->z[index];

                if (child->leaf == false) {
                    child->childs[(child->size) + 1] = right_sibling->childs[0];
                }

                this->x[index] = right_sibling->x[0];
                this->y[index] = right_sibling->y[0];
                this->z[index] = right_sibling->z[0];

                for (int i = 1; i < right_sibling->size; i++) {
                    right_sibling->x[i - 1] = right_sibling->x[i];
                    right_sibling->y[i - 1] = right_sibling->y[i];
                    right_sibling->z[i - 1] = right_sibling->z[i];
                }

                if (right_sibling->leaf == false) {
                    for (int i = 1; i <= right_sibling->size; i++) {
                        right_sibling->childs[i - 1] = right_sibling->childs[i];
                    }
                }

                child->size++;
                right_sibling->size--;
            }
            else {
                // – Subcase b: both subtree and immediate siblings have t-1 keys
                if (index == 0) {
                    // merge with right sibling
                    merge(index, index + 1);
                }
                else {
                    // merge with left sibling
                    merge(index - 1, index);
                }
            }
        }

        return this->childs[index]->remove(x, y, z, key);
        
    }

    return false;
}

void Node::merge(int left_index, int right_index) {
    Node *next_sibling = this->childs[right_index];

    this->childs[left_index]->x[this->size] = this->x[left_index];
    this->childs[left_index]->y[this->size] = this->y[left_index];
    this->childs[left_index]->z[this->size] = this->z[left_index];

    for (int i = 0; i < next_sibling->size; i++) {
        this->childs[left_index]->x[i + t] = next_sibling->x[i];
        this->childs[left_index]->y[i + t] = next_sibling->y[i];
        this->childs[left_index]->z[i + t] = next_sibling->z[i];
    }

    if (this->childs[left_index]->leaf == false) {
        for (int i = 0; i <= next_sibling->size; i++) {
            this->childs[left_index]->childs[i + t] = next_sibling->childs[i];
        }
    }

    for (int i = right_index; i < this->size; i++) {
        this->x[i - 1] = this->x[i];
        this->y[i - 1] = this->y[i];
        this->z[i - 1] = this->z[i];
    }

    for (int i = right_index + 1; i <= this->size; i++) {
        this->childs[i - 1] = this->childs[i];
    }

    this->childs[left_index]->size += next_sibling->size + 1;
    this->size--;

    delete next_sibling;
}

struct BTree {
    int node_count; // total node count
    int t; // degree of the tree
    char key; // key attribute (x, y or z)
    Node* root; // holds the top-most node

    BTree();
    ~BTree();

    void insert(int x, int y, char z);  // inserts new key to B-Tree
    void remove(int x, int y, char z);
    void print();   // prints tree
};

BTree::BTree() {
    root = NULL;
    t = 2;
    key = 'x';
    node_count = 0;
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

    node_count++;
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

void BTree::remove(int x, int y, char z) {
    if (root != NULL) {
        if (root->remove(x, y, z, this->key) == true) {
            this->node_count--;
        }
        
        if (node_count == 0) {
            Node *temp = root;
            if (root->leaf == true) {
                root = NULL;
            }
            else {
                root = root->childs[0];
            }
            delete temp;
        }
    }
}

int main() {
    BTree* new_tree = new BTree;

    int input_count = 0;

    scanf("%d", &input_count);
    scanf("%d", &new_tree->t);
    scanf(" %c", &new_tree->key);

    int x, y;
    char z;
    for (int i = 0; i < input_count; i++) {
        scanf("%d", &x);
        scanf("%d", &y);
        scanf(" %c", &z);
        new_tree->insert(x, y, z);
    }

    // get the key to be deleted
    if (new_tree->key == 'x') {
        scanf("%d", &x);
    } else if (new_tree->key == 'y') {
        scanf("%d", &y);
    } else if (new_tree->key == 'z') {
        scanf(" %c", &z);
    }
    new_tree->remove(x, y, z);

    new_tree->print();

    delete new_tree;

    return 0;
}