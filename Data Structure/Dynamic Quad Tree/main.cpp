#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <math.h>
using namespace std;

int grid[1024][1024],size=0;

struct Node {
    char status;Node* child[4]{NULL};
    void setStatus(char newstatus) {
        status = newstatus;
    };
    char getStatus() {
        return status;
    };
    void setChild(Node *p, int n){
        child[n] = p;
    }
    Node *getChild(int n) {
        return child[n];
    };
    char sameNo() {
        char result = getChild(0)->getStatus();
        for (int i = 1; i < 4; i++) {
            if (result != getChild(i)->getStatus()){
                return -1;
            }
        }
        return result;
    }
};

struct Tree {
    Node *root;
    int nodeNo = 0;
    Node* buildTree(int xCorncer, int yCorncer,int size) {
        Node* n = new Node();
        nodeNo++;
        if (size<2){
            n->setStatus(grid[xCorncer][yCorncer]);
            return n;
        }else{
            n->setChild(buildTree(xCorncer, yCorncer,size / 2), 0);
            n->setChild(buildTree(xCorncer+size/2, yCorncer,size / 2), 1);
            n->setChild(buildTree(xCorncer, yCorncer + size / 2,size / 2), 2);
            n->setChild(buildTree(xCorncer + size / 2, yCorncer + size / 2,size / 2), 3);
            char no = n->sameNo();
            n->setStatus(no);
            if(no=='0' || no == '1'){
                nodeNo -= 4;
            }
            return n;
        }
    }
    int getnodeNo() {
        return nodeNo;
    };
    void update(int x, int y,int xCorncer, int yCorncer, Node*n, int size) {
        Node *temp = n;
        if (size<2){
            if (temp->getStatus() == '1')
                temp->setStatus('0');
            else if(temp->getStatus()=='0')
                temp->setStatus('1');
        }else {
            if(x >= xCorncer+size/ 2){
                if (y >= yCorncer + size / 2) {
                    update(x, y, xCorncer+size / 2, yCorncer + size / 2, temp->getChild(3),size / 2);
                }else {
                    update(x, y, xCorncer + size / 2, yCorncer, temp->getChild(1), size / 2);
                }
            }else if (x < xCorncer+size/ 2) {
                if (y >= yCorncer + size / 2) {
                    update(x, y, xCorncer, yCorncer + size / 2, temp->getChild(2),size / 2);
                }else {
                    update(x, y, xCorncer, yCorncer, temp->getChild(0),size / 2);
                }
            }
            char oldstatus = temp->getStatus(),newstatus = temp->sameNo();
            temp->setStatus(newstatus);
            if (oldstatus != newstatus && (newstatus == '0' || newstatus == '1' )) {
                nodeNo -= 4;
            }else if (oldstatus != newstatus){
                nodeNo += 4;
            }
        }
    }
};

int main() {
    int cases,size_of_matrix,operations,R, C;
    string input;
    cin >> cases;
    for (int i = 0; i < cases; i++) {
        cin >> size_of_matrix;
        size = pow(2, size_of_matrix);
        for (int j = 0; j< size; j++) {
            cin >> input;
            for (int k = 0; k < size; k++)
                grid[j][k] = input[k];
        }
        Tree *t = new Tree();
        Node *root = t->buildTree(0, 0,size);
        cin >> operations;
        for (int j = 0; j < operations; j++) {
            cin >> R >> C;
            t->update(R-1, C-1, 0, 0, root,size);
            cout << t->getnodeNo() << endl;
        }
    }
    return 0;
}
