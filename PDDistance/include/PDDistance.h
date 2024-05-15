#include <iostream>
#include <vector>
#include <unordered_map>

typedef std::vector<double> POINT;
typedef std::vector<POINT> PD;

class Merge {
private:
    std::unordered_map<int, int> _UV;
    std::unordered_map<int, int> _VU;
public:
    Merge(){}
    void set(int u, int v) {
        _UV[u]=v;
        _VU[v]=u;
    }
    Merge(const Merge& other) {
        for (const auto& pair : other._UV) {
            _UV[pair.first] = pair.second;
        }
        for (const auto& pair : other._VU) {
            _VU[pair.first] = pair.second;
        }
    }
    int findU(int u) {
        auto it = _UV.find(u);
        if (it != _UV.end())
        {
            return it->second;
        }
        return -1;
    }    
    int findV(int v) {
        auto it = _VU.find(v);
        if (it != _VU.end())
        {
            return it->second;
        }
        return -1;
    }
    void erase(int u,int v) {
        _UV.erase(u);
        _VU.erase(v);
    }
    void update(const std::vector<int> &path) {
        if(path.size()==0) {
            return;
        }
        for (int i=0;i<path.size()-1;i++) {
            if (i%2==0) {
                set(path[i],path[i+1]);
            }
            else {
                erase(path[i+1],path[i]);
            }
        }
    }
    int size(){
        return _UV.size();
    }
    void print() {
        std::cout << "merge:{";
        for (auto it = _UV.begin(); it != _UV.end(); ++it) {
            std::cout << it->first << ": " << it->second;
            if (std::next(it) != _UV.end()) {
                std::cout << ", ";
            }
        }
        std::cout << "}" << std::endl;
    }
};

double distance(const std::vector<double> &point1, const std::vector<double> &point2);

enum DIRECTION{
    UToV,
    VToU,
    None
};

class Map {
private:
    int _dim=0;
    double** _data;
public:
    Map(){}
    Map(PD & pd1, PD & pd2);
    void init(PD&pd1,PD&pd2);
    Map(const Map & other);
    void copy(const Map & other);
    Map(Map && other);
    void move(Map && other);
    ~Map();
    int dim() const { return _dim;}
    double& at(int row,int col);
    void print();
    std::vector<double> sort();
    DIRECTION direction(int u, int v, Merge & merge, double epsilon);
    std::vector<int> onepath(Merge & merge,double epsilon);
    bool pathU(int input,Merge & merge,double epsilon,std::unordered_map<int, bool>& UVisited,std::unordered_map<int, bool> & VVisited,std::vector<int>& path);
    bool pathV(int input,Merge & merge,double epsilon,std::unordered_map<int, bool>& UVisited,std::unordered_map<int, bool> & VVisited,std::vector<int>& path);
    void maxmerge(Merge&merge,double epsilon);
    double bottleneck();
};

std::vector<int> dijkstra(const Map& value,const Map& direction);
