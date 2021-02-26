// oct tree testing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "print.h"

using namespace std;

struct Node {
    uint16_t pointer;
    uint16_t masks;
    int v;
    bool isLeaf;
    bool isValid;

};

bool isSame(Node* a) {
    char temp = a[0].v;
    for (int i = 1; i < 8; i++) {
        if (a[i].v != temp) {
            return false;
        }
    }
    return true;
}
bool isSame2(int* a) {
    int temp = a[0];
    for (int i = 1; i < 8; i++) {
        if (a[i]!= temp) {
            return false;
        }
    }
    return true;
}

ostream& operator << (ostream& os, Node t) {
    cout << "Pointer : " << t.pointer << endl;


    string temp1 = "";
    for (int i = 0; i < 8; i++) {
        temp1 += (t.masks & (1 << i)) ? "1" : "0";
    }
    string temp2 = "";
    for (int i = 0; i < 8; i++) {
        temp2 += (t.masks & (1 << 8 + i)) ? "1" : "0";
    }

    cout << "valid mask: " << temp1;
    cout << "  leaf mask: " << temp2 << endl;
    return os;
}

Node helper(char* voxels, list<int>& tree, int x,int y, int z, int w, int dim) {

    w /= 2;
    int pos = dim * dim * z + (y * dim + x);
    if (w == 0) {
        cout << "x: " << x << "  y: " << y << "  z: " << z << endl;
        cout << "pos: " << pos << endl;
        char v = voxels[pos];
        Node temp;
        temp.v = v;
        temp.isLeaf = true;
        temp.isValid = (v == 0) ? 0 : 1;
        return temp;

    }
    else {

        Node result[8];
        result[0] = helper(voxels, tree, x  ,y  , z  , w, dim);
        result[1] = helper(voxels, tree, x+w,y  , z  , w, dim);
        result[2] = helper(voxels, tree, x  ,y+w, z  , w, dim);
        result[3] = helper(voxels, tree, x+w,y+w, z  , w, dim);
        result[4] = helper(voxels, tree, x  ,y  , z+w, w, dim);
        result[5] = helper(voxels, tree, x+w,y  , z+w, w, dim);
        result[6] = helper(voxels, tree, x  ,y+w, z+w, w, dim);
        result[7] = helper(voxels, tree, x+w,y+w, z+w, w, dim);


        Node temp2;
        temp2.isLeaf = false;
        temp2.masks = 0;

        for (int i = 0; i < 8; i++) {
            temp2.masks |= ((result[i].isLeaf << (8+i)));
            temp2.masks |= ((result[i].isValid << (i)));

        }
        // test if all leaves. if Not set valid to true 
        if ((temp2.masks & 0xFF00)>>8 != 0xFF) {
            temp2.isValid = true;

        }
        else {
            temp2.pointer = -1;
            //all leaves therefor points no further
        }

        if (isSame(result)) {
            char v = result[0].v;
            if (v == 0) {

            }
            else {

            }
        }
        else {

        }
        return temp2;

    }

}

struct tree {
    int v;
    int p =-1;
    tree* nodes[8];
    int L = 0;
    char valid =0;
    char leaf =0;
    int parentind =-1;

    tree() {
        for (int i = 0; i < 8; i++) {
            nodes[i] = nullptr;
        }
    }

};

ostream& operator << (ostream& os, tree t) {
    cout << "v: " << t.v << " L: "<< t.L <<" P: "<< t.p << endl;
    cout << "valid mask: ";
    for (int i = 0; i < 8; i++) {
        cout << (((1<<i)& t.valid)? "1 ": "0 ");
    }
    cout <<endl<< "leaf mask: ";
    for (int i = 0; i < 8; i++) {
        cout << (((1 << i) & t.leaf) ? "1 " : "0 ");
    }


    cout << endl;
    cout << endl;
    return os;
}
int helper2(char* voxels, tree* node, int x, int y, int z, int w, int dim, int L = 0) {

    w /= 2;
    int pos = dim * dim * z + (y * dim + x);
    if (w == 0) {       
        node->v = voxels[pos];
        node->L = L;
        return voxels[pos];
    }
    else {
        for (int i = 0; i < 8; i++) {
            node->nodes[i] = new tree();
        }
        int result[8];
        result[0] = helper2(voxels, node->nodes[0], x, y, z, w, dim, L+1);
        result[1] = helper2(voxels, node->nodes[1], x + w, y, z, w, dim,L+1);
        result[2] = helper2(voxels, node->nodes[2], x, y + w, z, w, dim,L+1);
        result[3] = helper2(voxels, node->nodes[3], x + w, y + w, z, w, dim, L + 1);
        result[4] = helper2(voxels, node->nodes[4], x, y, z + w, w, dim, L + 1);
        result[5] = helper2(voxels, node->nodes[5], x + w, y, z + w, w, dim,L+1);
        result[6] = helper2(voxels, node->nodes[6], x, y + w, z + w, w, dim,L+1);
        result[7] = helper2(voxels, node->nodes[7], x + w, y + w, z + w, w, dim, L + 1);

        if (isSame2(result)) {
            node->v = result[0];
            node->L = L;
            return result[0];       
        }
        else {
            
            node->v = -1;
            node->L = L;
            return -1;
        }
        
    }
}

void buildTree(char* voxels, list<int>& tr,int dim) {
    helper(voxels, tr, 0, 0, 0, dim, dim);

    


}

int X = 0;
void bfs(tree* t, vector<tree*>& cd) {

    queue<tree*> q;
    q.push(t);

    while (!q.empty()) {

        tree* c = q.front();
        bool added = false;
        if (X == 0) {
            cd.push_back(c);
            added = true;

        }
        else {
            if (c->v == -1) {
                cd.push_back(c);
                added = true;


            }
        }

        q.pop();

        if (c->v == -1) {
            int ind = cd.size() - 1;
            for (int i = 0; i < 8; i++) {
                if (c->nodes[i]) {
                    c->nodes[i]->parentind = X;
                    q.push(c->nodes[i]);
                }

            }
        }
        if (added)
            X += 1;


    }

}
vector<tree*> buildTree2(char* voxels, tree* tr, int dim) {
    helper2(voxels, tr, 0, 0, 0, dim, dim);

    vector<tree*> cd;
    bfs(tr, cd);
    for (int i = 0; i < cd.size(); i++) {
        if (cd[i]->parentind >= 0) {
            if (cd[cd[i]->parentind]->p == -1) {
                cd[cd[i]->parentind]->p = i - cd[i]->parentind;
            }
        }

        for (int j = 0; j < 8; j++) {
            cd[i]->valid |= (cd[i]->nodes[j]->v != 0)<<j;
            cd[i]->leaf |= (cd[i]->nodes[j]->v != -1)<<j;
        }
    }

    return cd;
}


int main()
{
    int n = pow(8, 3);
    char* voxels = new char[n];


    tree* root = new tree();


    for (int i = 0; i < n; i++) {
        voxels[i] = 0;
    }
    voxels[0] = 1;
    /*voxels[1] = 1;
    voxels[8] = 1;
    voxels[9] = 1;
    voxels[64] = 1;
    voxels[65] = 1;
    voxels[72] = 1;
    voxels[73] = 1;*/
 
   
    

    //cout << dog;
   
    vector<tree*> cd = buildTree2(voxels, root, 8);
    

    for (auto t : cd) {

        cout << *t;

    }

    //16bit //need to store 16bit relativepointer
    //16bit //two masks for valid and leaf each 8 bit


    
}

