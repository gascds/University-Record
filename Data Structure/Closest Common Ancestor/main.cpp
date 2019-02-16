#include <iostream>
using namespace std;

class Node {
public:
    int num;
    int childNo = 0;
    Node(int n){
        num = n;
    };
    Node* child[101]{ NULL };
    Node* parent = NULL;
    void addChild(int n){
        Node* temp = new Node(n);
        child[childNo] = temp;
        childNo++;
    };
    void setParent(Node *temp){
        parent = temp;
    };
};

class Tree {
public:
    Node *root;
    Tree(int n){
        Node * temp = new Node(n);
        root = temp;
    };
    Node* findNode(int,Node*);
    int depth(Node*);
    int output(int,int);
};

Node* Tree::findNode(int n, Node *in) {
    Node *temp;
    if (in != NULL) {
        if (n == in->num)
            return in;
        else {
            int i = 0;
            while(in->child[i] != NULL){
                temp = findNode(n,in->child[i]);
                if (temp != NULL)
                    return temp;
                i++;
            }
        }
    }
    return NULL;
}

int Tree::depth(Node *in) {
    if (in->parent == NULL)
        return 0;
    else
        return (depth(in->parent)+1);
}

int Tree::output(int first, int last) {
    Node *temp1 = findNode(first,root);
    Node *temp2 = findNode(last,root);
    int a = depth(temp1);
    int b = depth(temp2);
    int c = abs(a - b);
    
    if (c>0 && a < b) {
        for (int i = 0; i < c; i++)
            temp2 = temp2->parent;
    }else if(c>0 && a>b)
        for (int i = 0; i < c; i++)
            temp1 = temp1->parent;
    
    while (temp1 != NULL) {
        if (temp1->num == temp2->num)
            return temp1->num;
        temp1 = temp1->parent;
        temp2 = temp2->parent;
    }
    return 0;
}

int main() {
    int cases,root, no,child, parent,first, second;
    cin >> cases;
    for (int i = 0; i < cases; i++) {
        cin >> root >> no;
        Tree *tree = new Tree(root);
        Node* root = tree->root;
        for (int j = 0; j < no - 1; j++) {
            cin >> child >> parent;
            Node* temp= tree->findNode(parent, root);
            temp->addChild(child);
            temp->child[temp->childNo-1]->setParent(temp);
        }
        cin >> first>> second;
        cout << tree->output(first, second)<<endl;
        delete tree;
    }
    return 0;
}
