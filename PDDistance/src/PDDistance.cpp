#include <cmath>
#include <algorithm>
#include <limits>
#include <unordered_map>
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
    // std::cerr<<"no path found"<<std::endl;
    return path;
}

bool Map::pathV(int v,Merge & merge,double epsilon,std::unordered_map<int,bool> &UVisited,std::unordered_map<int,bool> &VVisited,std::vector<int>& path) {
    VVisited[v]=true;
    path.push_back(v);
    if(merge.findV(v)==-1){
        return true;
    }
    for(int u=0;u<_dim;u++) {
        if(direction(u,v,merge,epsilon)==VToU){
            if(UVisited.find(u)==UVisited.end()){
                if(pathU(u,merge,epsilon,UVisited,VVisited,path)){
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
    int down = sortedge[0];
    int size = sortedge.size();
    int up = size-1;
    int epsilonindex = floor((down+up)/2.);
    int count=0;
    while(true){
        Merge merge;
        double epsilon = sortedge[epsilonindex];
        maxmerge(merge, epsilon);
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
int min(const std::vector<int>& unfinish,const std::unordered_map<int, double>& gamma){
    int minindex = -1;
    double minvalue = std::numeric_limits<double>::infinity();
    for(const auto& pair:gamma){
        if(std::find(unfinish.begin(),unfinish.end(),pair.first)!=unfinish.end()){
            if(pair.second < minvalue&& pair.second != -1)
            {
                minvalue = pair.second;
                minindex = pair.first;
            }
        }
    }
    return minindex;
}
void Map::dijkstra(
    Merge& merge,
    std::vector<int>& mincostpath,
    std::unordered_map<int, double>&du,
    std::unordered_map<int, double>&dv){
    std::vector<int> finishu;
    std::vector<int> finishv;
    std::vector<int> unfinishu;
    std::vector<int> unfinishv;
    std::unordered_map<int, double> gammau;
    std::unordered_map<int, double> gammav;
    for(int u=0;u<_dim;u++) {
        unfinishu.push_back(u);
        unfinishv.push_back(u);
        gammav[u]=-1;
        if(merge.findU(u)==-1) {
            gammau[u]=0;
        }
        else {
            gammau[u]=-1;
        }
    }

    while(unfinishu.size()!=0||unfinishv.size()!=0){
        auto minu = min(unfinishu,gammau);
        auto minv = min(unfinishv,gammav);

        if ((minu!=-1&&minv!=-1&&(gammau[minu] < gammav[minv]))||minv==-1) {
            finishu.push_back(minu);
            unfinishu.erase(std::remove(unfinishu.begin(),unfinishu.end(),minu),unfinishu.end());
            for(int v = 0; v<_dim;v++) {
                if(std::find(unfinishv.begin(),unfinishv.end(),v)!=unfinishv.end())
                {
                    if(direction(minu,v,merge,std::numeric_limits<double>::infinity())==UToV) {
                        double newgamma = gammau[minu]+_data[minu][v]-du[minu]-dv[v];
                        if (gammav[v]>newgamma||gammav[v]==-1) {
                            gammav[v] = newgamma;
                        }
                    }
                }
            }
        }
        else if(minu==-1&&minv==-1){std::cerr<<"dijkstra wrong"<<std::endl;break;}
        else {
            finishv.push_back(minv);
            unfinishv.erase(std::remove(unfinishv.begin(),unfinishv.end(),minv),unfinishv.end());
            for(int u = 0; u<_dim;u++) {
                if(std::find(unfinishu.begin(),unfinishu.end(),u)!=unfinishu.end())
                {
                    if(direction(u,minv,merge,std::numeric_limits<double>::infinity())==VToU) {
                        double newgamma = gammav[minv]+_data[u][minv]-du[u]-dv[minv];
                        if (gammau[u]>newgamma||gammau[u]==-1) {
                            gammau[u] = newgamma;
                        }
                    }
                }
            }
        }
    }

    int tos=-1;
    double tosvalue = std::numeric_limits<double>::infinity();
    for(int v=0; v<_dim;v++){
        if(merge.findV(v)==-1){
            if(gammav[v]<tosvalue){
                tosvalue = gammav[v];
                tos = v;
            }
        }
    }
    if(tos==-1){
        std::cerr<<"minimum cost value not found"<<std::endl;
    }
    else{
        bool finish=true;
        for(int u=0;u<_dim;u++){
            if(merge.findU(u)==-1)
            {
                finish=false;
                break;
            }
        }
        if(!finish){
            bool inV = true;
            int index = tos;
            mincostpath.clear();
            mincostpath.push_back(index);
            int count = 0;
            while(true){
                // std::cout<<"begin-inV:"<<inV<<std::endl;
                if(inV==true){
                    for(int u=0;u<_dim;u++){
                        if(direction(u,index,merge,std::numeric_limits<double>::infinity())==UToV){
                            if(gammau[u]+_data[u][index]-du[u]-dv[index]==gammav[index]){
                                inV = false;
                                index=u;
                                mincostpath.push_back(u);
                                break;
                            }
                        }
                    }
                }
                else if(inV==false){
                    if(merge.findU(index)==-1){
                        std::reverse(mincostpath.begin(),mincostpath.end());
                        return;
                    }
                    for(int v=0;v<_dim;v++){
                        if(direction(index,v,merge,std::numeric_limits<double>::infinity())==VToU){
                            if(gammav[v]+_data[index][v]-du[index]-dv[v]==gammau[index]){
                                count++;
                                index=v;
                                inV=true;
                                mincostpath.push_back(v);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    for(int u = 0; u<_dim;u++){
        du[u]-=gammau[u];
    }
    for(int v = 0; v<_dim;v++){
        dv[v]+=gammav[v];
    }
}

double Map::wasserstein(int p){
    std::unordered_map<int, double>du;
    std::unordered_map<int, double>dv;
    for(int u = 0; u<_dim;u++){
        du[u]=0;
        dv[u]=0;
    }
    std::vector<int> minimumcostpath{};
    Merge merge;
    maxmerge(merge,0);
    do{
        dijkstra(merge,minimumcostpath,du,dv);
        merge.update(minimumcostpath);
    }
    while(merge.size()<_dim);
    double wasserstein = 0;
    if(p!=-1){
        for(int u = 0; u<_dim;u++){
            wasserstein+=pow(_data[u][merge.findU(u)],p);
        }   
        return pow(wasserstein,1.0/p);
    }
    else{
        for(int u = 0; u<_dim;u++){
            if(wasserstein<_data[u][merge.findU(u)]){
                wasserstein=_data[u][merge.findU(u)];
            }
        }   
        return wasserstein;
    }
}