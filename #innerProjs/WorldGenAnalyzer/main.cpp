#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <windows.h>
#include <map>

using namespace std;

string out = "";
int genType(int x,int y,int seed){
    return (int)((3*x + 4*y)/(seed/124.0) + x*x - y*y);
}

struct Point{
    int x,y;
    bool operator ==(Point & b){
        if(b.x == x && b.y == y)return true;
        return false;
    }
};

struct Ret{
    int v;
    int e;
};

Ret tryAccess(int dd[9][9],int x,int y){
    if(x < 0 || y < 0 || x >= 9 || y >= 9)return {0,1};
    return {dd[x][y],0};
}
/*
int tried[9][9];
    int visited[9][9];
    int index = 0;
    vector<Point> waited = {};
    vector<Point> newI = {{0,0}};
    map<int,vector<Point>> connected;
    int triedNum = -1;
    memset(tried,0,81 * sizeof(int));
    memset(visited,0,81 * sizeof(int));
    while(!newI.empty()){
        vector<Point> ppm;
        bool ext;
        Point root = newI[newI.size()-1];
        ppm.push_back(root);
        newI.pop_back();
        waited = {root};
        triedNum = dta[root.x][root.y];
        int l = 1;
        while(!waited.empty()){
            Point pp = waited[waited.size()-1];
            ext = false;
            waited.pop_back();
            if(tried[pp.x][pp.y] != 1){
                tried[pp.x][pp.y] = 1;
                for(auto &x : connected){
                    for(Point & pxm : x.second){
                        if(pp == pxm){
                            ext = true;
                            break;
                        }
                    }
                    if(ext)break;
                }
                if(ext)break;
                for(int mpx = -1;mpx < 2;++mpx){
                    for(int mpy = -1;mpy < 2;++mpy){
                        Ret tp = tryAccess(dta,pp.x + mpx,pp.y + mpy);
                        if(!tp.e){
                            if(tp.v == triedNum){
                                bool env = true;
                                for(Point & p : waited){
                                    if(p.x == pp.x + mpx && p.y == pp.y+mpy)env = false;
                                }
                                if(env && tried[pp.x + mpx][pp.y + mpy] != 1){
                                    visited[pp.x + mpx][pp.y + mpy] = 1;
                                    waited.push_back({pp.x + mpx,pp.y + mpy});
                                    l++;
                                    ppm.push_back({pp.x + mpx,pp.y + mpy});
                                }
                            }else{
                                bool env = true;
                                for(Point & p : newI){
                                    if(p.x == pp.x + mpx && p.y == pp.y+mpy)env = false;
                                }
                                if(env && tried[pp.x + mpx][pp.y + mpy] != 1 && visited[pp.x+mpx][pp.y+mpy] != 1){
                                   newI.push_back({pp.x + mpx,pp.y + mpy});
                                }
                            }
                        }
                    }
                }
            }
        }
        if(ext)break;
        connected.insert(make_pair(index++,ppm));
        out += "Root (";
        out += to_string(root.x);
        out += string(",");
        out += to_string(root.y) + string(") is connected with ") + to_string(l) + string(" chunks.\n");
    }
    out += "Finished\n\n";
*/
void Analyze(int dta[9][9]){
    /*int mask[9][9];
    vector<Point> waited,newAccs,cnss;
    map<int,vector<Point>> cns;
    int index = 0;
    Point root,pp;
    newAccs = {{0,0}};
    memset(mask,0,81 * sizeof(int));
    while(!newAccs.empty()){
        waited.push_back(newAccs[newAccs.size()-1]);
        root = waited[0];
        cnss = {root};
        mask[root.x][root.y] = 1;
        while(!waited.empty()){
            pp = waited[waited.size()-1];
            for(int mpx = -1;mpx < 2;++mpx){
                for(int mpy = -1;mpy < 2;++mpy){
                    if(mask[pp.x + mpx][pp.y + mpy] == 1)continue;
                    mask[pp.x + mpx][pp.y + mpy] = 1;
                    Ret tp = tryAccess(dta,pp.x + mpx,pp.y + mpy);
                    if(!tp.e){
                        if(tp.v == dta[root.x][root.y]){
                            waited.push_back({pp.x + mpx,pp.y + mpy});
                            cnss.push_back({pp.x + mpx,pp.y + mpy});
                        }else{
                            newAccs.push_back({pp.x + mpx,pp.y + mpy});
                        }
                    }
                }
            }
        }
        cns.insert(make_pair(index++,cnss));
        out += "Root (";
        out += to_string(root.x);
        out += string(",");
        out += to_string(root.y) + string(") is connected with ") + to_string(cnss.size()) + string(" chunks.\n");
    }
    out += "Finished\n\n";*/
}

int main()
{
    srand(time(0));
    vector<int> seeds;
    int singleChunk [9][9];
    memset(singleChunk,0,81 * sizeof(int));
    cout << "Starting to run..." << endl;
    ///Gen Seed
    for(int i = 0;i < 1;){
        int x = rand() - INT_MAX/3;
        for(auto & y : seeds){
            if(y == x)continue;
        }
        seeds.push_back(x);
        ///Test Seed
        out += string("---------------------------\n") + "Current Seed:" + to_string(x) + "\n";
        for(int z = 0;z < 1;++z){
            int px = rand()-INT_MAX/3,py = rand() - INT_MAX/3;
            out += string("Seed Map (") + to_string(px+4) + "," + to_string(py+4) + "):\n";
            for(int iy = 0;iy < 9;++iy){
                for(int ix = 0;ix < 9;++ix){
                    singleChunk[ix][iy] = abs(genType(px+ix,py+iy,x) % 2);
                    out += to_string(singleChunk[ix][iy]) + " ";
                }
                out += "\n";
            }
            out += "Analyze:\n";
            Analyze(singleChunk);
        }
        ++i;
    }
    cout << "Terminated..Stored In data.txt\n";
    ofstream of("data.txt");
    of << out;
    of.close();
    return 0;
}
