// oct tree testing.cpp : This file contains the 'main' function. Program execution begins and ends there.
#define OLC_PGE_APPLICATION

#include <iostream>
#include "print.h"
#include "olcPixelGameEngine.h"
#include "vf3dMat44.h"

typedef vf3d v3;

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


v3 project(v3 v, float FOV, int w, int h) {
    float k = tan(FOV / 2) * v.z;
    float x = ((v.x / k) + 1.0)/2.0;
    float y = ((v.y / k) + 1.0)/2.0;


    return { x * w, y * h, v.z };
}

struct cube {
    static v3 points[8];
    static int mappings[24];


    static void DrawCube(v3& cam, mat44& camorient, olc::PixelGameEngine* scr, v3 pos, v3 dim) {

        



    }

};

v3 getxyz(int i, int dim) {
    int dim1 = dim * dim;
    return { (float)(i % dim), (float)(i % dim1 / dim),(float)(i / dim1) };
}

struct d3 : olc::PixelGameEngine {
    v3 cam = { 0,0,0 };
    v3 voxelGridpos = { 0,0,80 };
    mat44 orient = mat44::CreateIdentity();
    mat44 orientinv;
    int n;
    int dim = 64;
    char* voxels;
    float FOV = 1.2;


    v3 cubepoints[8] = { {0,0,0},{1,0,0},{0,1,0},{1,1,0},{0,0,1},{1,0,1},{0,1,1},{1,1,1} };
    int cubelinemappings[24] = { 0,1,0,2,0,4,5,4,5,7,5,1 ,6,7,6,4,6,2,3,7,3,1,3,2 };

    vector<tree*> cd;
    tree* root = nullptr;

    float r = 5;
    float s = 0.0;


    void DeleteTree(tree* t) {
        if (t) {
            for (int i = 0; i < 8; i++) {
                if (t->nodes[i]) {
                    DeleteTree(t->nodes[i]);
                }

            }
            
             delete t;
        }
        
    }

    void InitOct() {
        for (int i = 0; i < n; i++) {
            voxels[i] = 0;
        }
        //DeleteTree(root);

        root = new tree();
        for (int i = 0; i < n; i++) {
            if (rand() % 10000 == 1) {
                voxels[i] = 1;
            }
        }
        /*for (int i = 0; i < n; i++) {

            v3 v = getxyz(i, dim);
            float s = (float)(dim / 2);
            if ((v - v3(s, s, s)).mag() < r ) {
                voxels[i] = 1;
            }
        }*/
        voxels[100] = 1;
        voxels[1400] = 1;
        cd = buildTree2(voxels, root, dim);
        
    }

    bool OnUserCreate() {
       
        n  = pow(dim, 3);
        voxels = new char[n];
        InitOct();

        
        /*voxels[1] = 1;
        voxels[7] = 1;
        voxels[56] = 1;
        voxels[63] = 1;
        voxels[200] = 1;*/
        /*voxels[200] = 1;
        voxels[448] = 1;
        voxels[455] = 1;
        voxels[504] = 1;
        voxels[511] = 1;*/
        //cout << dog;

        
        for (auto t : cd) {

            cout << *t;

        }

        return true;
    }
    void DrawCube(v3 pos, float dim, int L) {
        

        float darkness = (float)L / 5.0;
        int c = darkness * 255;
        for (int i = 0; i < 24; i += 2) {

            v3 p1 = pos +cubepoints[cubelinemappings[i]] * dim;
            v3 p2 = pos+ cubepoints[cubelinemappings[i + 1]] * dim;
            
           
            p1 += voxelGridpos;
            p1 = orientinv * (p1 - cam);
            
            p1 = project(p1, FOV, ScreenWidth(), ScreenHeight());

            p2 += voxelGridpos;
            p2 = orientinv * (p2 - cam);
            
            p2 = project(p2, FOV, ScreenWidth(), ScreenHeight());


            

            DrawLine(p1.x, p1.y, p2.x, p2.y, olc::Pixel(c,c,c));


        }


    }

    void DrawVoxels() {
        int dim1 = dim * dim;
        for (int i = 0; i < dim * dim * dim; i++) {
            if (voxels[i]) {
                v3 pos = { (float)(i % dim), (float)(i % dim1 / dim),(float)(i / dim1) };
                pos += v3(0.5, 0.5, 0.5);
                pos += voxelGridpos;
                pos = orientinv * (pos - cam);
                pos = project(pos, FOV, ScreenWidth(), ScreenHeight());
                
                FillCircle(pos.x, pos.y, 5, olc::RED);
            }
            
            

        }
    }
    vector<bool> nextpos = { 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1 };
    void recurse(int p, v3 pos, float dim) {
        tree* current = cd[p];

        DrawCube(pos, dim, current->L);
        //count children
        int cc = 0;
        vector<int> inds;
        for (int i = 0; i < 8; i++) {
            if (!((1 << i) & current->leaf)) {
                
                inds.push_back(i);
                
            }

            if (((1 << i) & current->valid)) {
                float xw = nextpos[i * 3] ? dim/2 : 0;
                float yw = nextpos[i * 3 + 1] ? dim/2 : 0;
                float zw = nextpos[i * 3 + 2] ? dim/2 : 0;
                v3 npos = pos + v3(xw, yw, zw);
                DrawCube(npos, dim/2, current->L);
            }
                
        }
     
        int x = 0;
        dim /= 2;
        for (auto& t : inds) {
            
            float xw = nextpos[t * 3] ? dim : 0;
            float yw = nextpos[t * 3 + 1] ? dim : 0;
            float zw = nextpos[t * 3 + 2] ? dim : 0;
            v3 npos = pos + v3(xw, yw, zw);
            
            recurse(p + current->p + x, npos, dim);

            x++;

        }


    }
    
    void DrawOctTree(int dim) {

        v3 pos = { 0,0,0 };
        int p = 0;
        
           
        recurse(0, pos, dim);
            

        
        




    }


    float speed = 20;
    bool OnUserUpdate(float ft) {
        //InitOct();
        s += ft*0.01;

        orientinv = orient.Invert();
        Clear(olc::BLACK);
        if (GetKey(olc::A).bHeld) {
            cam -= orient.GetBasisX()* speed *ft;
        }
        if (GetKey(olc::D).bHeld) {
            cam += orient.GetBasisX() * speed * ft;
        }
        if (GetKey(olc::W).bHeld) {
            cam += orient.GetBasisZ() * speed * ft;
        }
        if (GetKey(olc::S).bHeld) {
            cam -= orient.GetBasisZ() * speed * ft;
        }
        if (GetKey(olc::SPACE).bHeld) {
            cam.y -= speed * ft;
        }
        if (GetKey(olc::CTRL).bHeld) {
            cam.y += speed * ft;
        }


        if (GetKey(olc::RIGHT).bHeld) {

            orient = mat44::CreateYRot(speed*0.08 * ft) * orient;

        }
        if (GetKey(olc::LEFT).bHeld) {

            orient = mat44::CreateYRot(-speed * 0.08 * ft) * orient;

        }



        //DrawVoxels();
        //DrawCube({ 0,0,0 }, 8);

        DrawOctTree(dim);

        return true;


    }
};




int main()
{
    
    d3 test;
    test.Construct(600, 600, 1, 1);
    test.Start();

    

    //16bit //need to store 16bit relativepointer
    //16bit //two masks for valid and leaf each 8 bit


    
}

