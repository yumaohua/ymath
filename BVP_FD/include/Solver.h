#ifndef SOLVER
#define SOLVER
#include <vector>
#include <lapacke.h>
#include <unordered_map>
#include <fstream>
#include "./Domain.h"
#include "./Assistance.h"
#include "./Function.h"
#include "./Tensor.h"
/// @brief  求解器类
class Solver{
    private:
    /// @brief 定义域类
    Domain _domain;
    /// @brief 右侧函数
    Function* _rhs_function=nullptr;
    /// @brief 每一维格点数-1(包括端点)
    int _grid_number;
    /// @brief 输出文件名称
    std::string _output_name;
    /// @brief 线性系统系数z矩阵
    Matrix _A;
    /// @brief 线性系统右侧向量
    Vector _b;
    /// @brief 将离散点的格点坐标(i,j)与其是第几个离散点对应,顺序为从左到右,从下到上
    std::unordered_map<std::string,int> MatToVec{};
    /// @brief MatToVec的反向对应
    std::unordered_map<int,std::vector<int>> VecToMat{};
    /// @brief solution error
    std::vector<double> error;
    public:
    Solver(){}
    /// @brief 判断各组件准备情况(未完善,本想方便测试,实际上没什么用)
    /// @return 是否准备好
    bool Prepare();
    /// @brief 初始化
    /// @param rhs_function 右端函数
    /// @param grid_number  每一维格点数减1
    /// @param output_name 输出文件名字
    /// @param if_sub 是否在定义域减去圆
    /// @param type 总体而言是什么边界条件,如dirichlet,neumann,mixed
    /// @param value_func 非圆边界函数
    /// @param type_func 非圆边界类型函数,返回0表示未定义,返回1表示该点是dirichlet条件,返回2表示该点是neumann条件
    /// @param radius 圆周半径
    /// @param center 圆中心点坐标
    /// @param ctype 圆的总体边界条件类型
    /// @param cvalue_func 圆的边界函数
    /// @param ctype_func 圆边界的类型函数
    void Initialize(
    Function* rhs_function,
    int grid_number,
    std::string output_name,
    bool if_sub,
    const std::string& type,
    Function* value_func,
    Function* type_func = nullptr,
    double radius=0,
    const std::vector<double>& center=std::vector<double>{},
    const std::string& ctype="",
    Function* cvalue_func=nullptr,
    Function* ctype_func = nullptr
    );
    /// @brief 完成_A,_b赋值
    void Define();
    /// @brief 获取格点序号为(i,j)的线性方程的系数
    /// @param i 格点横序号
    /// @param j 格点纵序号
    /// @return 返回左上右下中点的系数,以及右侧常数的系数或值
    std::vector<double> DefineCoef(int i, int j);
    /// @brief 解线性方程组
    void Solve();
    /// @brief 为MatToVec和VecToMat赋值
    void IndexTransformation();
    /// @brief 返回求得的解
    /// @return 计算解
    const Vector & GetSolution() const {return _b;}
    /// @brief 输出到txt文件
    void Output();
    /// @brief 设置误差
    /// @param RealSolution 真实解
    void SetError(Function* RealSolution);
    /// @brief 计算1-norm
    /// @return 1-norm值 
    double ErrorOneNorm();
    /// @brief 计算2-norm
    /// @return 2-norm值
    double ErrorTwoNorm();
    /// @brief 计算max-norm
    /// @return max-norm值
    double ErrorMaxNorm();
    int maxerrorplace();
};
#endif