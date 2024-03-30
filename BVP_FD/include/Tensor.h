#ifndef TENSOR
#define TENSOR
#include <vector>
#include <cmath>
template<typename T, int Order>
class Tensor{
private:
    std::vector<T> __a;
    int __size;

public:
    Tensor():__size(0){}
    void initialize(int n){
        __size = n;
        int dataSize = std::pow(n,Order);
        __a.resize(dataSize);
    }
    T* data(){
        return &__a[0];
    }
    int size() const{
        return __size;
    }
    void VSetValue(int i, T value)
    {
        __a[i]=value;
    }
    void MSetValue(int i, int j, T value)
    {
        __a[i*size()+j]=value;
    }
    T MValue(int i,int j){
        return __a[i*size()+j];
    }
    T Mshow(int i,int j)
    {
        return __a[i*size()+j];
    }
    const T& operator()(int i)const  requires (Order==1){
        return __a[i];
    }
    T& operator()(int i) requires (Order==1) {
        return __a[i];
    }
    const T& operator()(int i,int j)const requires(Order==2) {
        return __a[i*size()+j];
    }
    T& operator()(int i,int j)requires(Order==2) {
        return __a[i*size()+j];
    }
};
typedef Tensor<double,1> Vector;
typedef Tensor<double,2> Matrix;
#endif