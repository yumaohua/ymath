#include <cmath>
#include <algorithm>
#include "../include/PDDistance.h"

double distance(const std::vector<double> &point1, const std::vector<double> &point2) {
    if (point1[0]==point1[1]) {
        if (point2[0]==point2[1]) {
            return 0;
        }
        else {
            return abs(point2[1]-point2[0])*1.0/2;
        }
    }
    else {
        if (point2[0]==point2[1]) {
            return abs(point1[1]-point1[0])*1.0/2;
        }
        else {
            return fmax(abs(point1[1]-point2[1]),abs(point1[0]-point2[0]));
        }
    }
}


Map::Map(PD&pd1, PD&pd2) {
    init(pd1,pd2);
}

void Map::init(PD&pd1, PD&pd2) {
    int pd1num=pd1.size();
    int pd2num=pd2.size();
    _dim = pd1num+pd2num;
    std::vector<double> diagpoint = {1,1};
    _data = new double*[_dim];
    for (int i=0;i<_dim;i++) {
        _data[i] = new double[_dim];
        for (int j=0;j<_dim;j++) {
            if (i<pd1num) {
                if (j<pd2num) {
                    _data[i][j] = distance(pd1[i],pd2[j]);
                }
                else {
                    _data[i][j] = distance(pd1[i],diagpoint);
                }
            }
            else {
                if (j<pd2num) {
                    _data[i][j] = distance(pd2[j],diagpoint);
                }
                else {
                    _data[i][j] = 0;
                }
            }
        }
    }
}

Map::Map(const Map& other){
    copy(other);
}

void Map::copy(const Map& other) {
    _dim = other._dim;
    _data = new double*[_dim];
    for (int i=0;i<_dim;i++) {
        _data[i] = new double[_dim];
        for (int j=0;j<_dim;j++) {
            _data[i][j] = other._data[i][j];
        }
    }
}

Map::Map(Map&&other){
    _dim = other._dim;
    _data = other._data;
    other._data = nullptr;
    other._dim = 0;
}

void Map::move(Map&&other) {
    _dim = other._dim;
    _data = other._data;
    other._data = nullptr;
    other._dim = 0;
}

Map::~Map() {
    for (int i=0;i<_dim;i++) {
        delete[] _data[i];
    }
    delete[] _data;
}


double& Map::at(int row,int col) {
    return _data[row][col];
}

void Map::print() {
    for (int i=0;i<_dim;i++) {
        for (int j=0;j<_dim;j++) {
            std::cout<<_data[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

std::vector<double> Map::sort() {
    std::vector<double> res;
    for (int i=0;i<_dim;i++) {
        for (int j=0;j<_dim;j++) {
            res.push_back(_data[i][j]);
        }
    }
    std::sort(res.begin(),res.end());
    return res;
}


DIRECTION Map::direction(int u, int v, Merge & merge, double epsilon) {
    if(u==-1||v==-1) {
        return None;
    }
    if(_data[u][v] > epsilon) {
        return None;
    }
    auto it = merge.findU(u);
    if(it!= -1) {
        if(it==v) {
            return VToU;
        }
    }
    return UToV;
}


std::vector<int> Map::onepath(Merge & merge,double epsilon) {
    std::unordered_map<int,bool> UVisited;
    std::unordered_map<int,bool> VVisited;
    std::vector<int> path{};
    for(int begin=0;begin<_dim;begin++) {
        if(merge.findU(begin)==-1)
        {
            if(UVisited.find(begin)==UVisited.end()) {
                UVisited[begin]=true;
            }
            path.push_back(begin);
            for(int v=0;v<_dim;v++) {
                if(direction(begin,v,merge,epsilon)==UToV){
                    if (VVisited.find(v)==VVisited.end()) {
                        if (pathV(v,merge,epsilon,UVisited,VVisited,path)){
                            std::cout<<"begin:"<<begin<<std::endl;
                            return path;
                        }
                        else{
                            continue;
                        }
                    }
                }
                else if(direction(begin,v,merge,epsilon)==None){
                    continue;
                }
            }
            path.pop_back();
        }
    }
    std::cerr<<"no path found"<<std::endl;
    return path;
}

bool Map::pathV(int v,Merge & merge,double epsilon,std::unordered_map<int,bool> &UVisited,std::unordered_map<int,bool> &VVisited,std::vector<int>& path) {
    VVisited[v]=true;
    path.push_back(v);
    if(merge.findV(v)==-1){
        std::cout<<"end:"<<v<<"-";
        return true;
    }
    for(int u=0;u<_dim;u++) {
        if(direction(u,v,merge,epsilon)==VToU){
            if(UVisited.find(u)==UVisited.end()){
                if(pathU(u,merge,epsilon,UVisited,VVisited,path)){
                    std::cout<<"v:"<<v<<"-";
                    return true;
                }
                else{
                    continue;
                }
            }
        }
        else if(direction(u,v,merge,epsilon)==None){
            continue;
        }
    }
    path.pop_back();
    return false;
}
bool Map::pathU(int u,Merge & merge,double epsilon,std::unordered_map<int,bool> &UVisited,std::unordered_map<int,bool> &VVisited,std::vector<int>& path) {
    UVisited[u]=true;
    path.push_back(u);
    for(int v=0;v<_dim;v++) {
        if(direction(u,v,merge,epsilon)==UToV){
            if(VVisited.find(v)==VVisited.end()){
                if(pathV(u,merge,epsilon,UVisited,VVisited,path)){ 
                    std::cout<<"u:"<<u<<"-";
                    return true;
                }
                else{
                    continue;
                }
            }
        }
        else if(direction(u,v,merge,epsilon)==None){
            continue;
        }
    }
    path.pop_back();
    return false;
}

void Map::maxmerge(Merge&merge,double epsilon){
    while(true){
        std::vector<int> path = onepath(merge,epsilon);
        if(path.size()==0){
            break;
        }
        merge.update(path);
    }
}

double Map::bottleneck(){
    std::vector<double> sortedge = sort();
    // for(auto& edge : sortedge){
    //     std::cout<<edge<<std::endl;
    // }
    int down = sortedge[0];
    int size = sortedge.size();
    int up = size-1;
    int epsilonindex = floor((down+up)/2.);
    int count=0;
    while(true){
        Merge merge;
        double epsilon = sortedge[epsilonindex];
        maxmerge(merge, epsilon);
        merge.print();
        std::cout<<"epsilon:"<<epsilonindex<<std::endl;
        std::cout<<"--------------------------------"<<std::endl;
        if(merge.size() == _dim){
            up = epsilonindex;
            if(up-down==1){
                return epsilon;
            }
            epsilonindex = floor((down+up)/2.);
        }
        else{
            down = epsilonindex;
            epsilonindex = floor((down+up)/2.);
        }
        count++;
        if(count==_dim){
            maxmerge(merge, sortedge[epsilonindex+1]);
            if(merge.size() == _dim){
                return sortedge[epsilonindex];
            }
            std::cerr<<"bottleneck wrong"<<std::endl;
            break;
        }
    }
    return -1;
}