#ifndef EXPRESSION_TREE
#define EXPRESSION_TREE
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
/// @brief 所有运算的枚举
enum Operators {
    Variable,
    Equal,
    Add,
    Minus,
    Times,
    DividedBy,
    Abs,
    None
};
/// @brief c++枚举不能是字符串所以设置了这个,有空换成Hashmap
const std::vector<std::string> SOperators {
    "Variable",
    "Equal",
    "Add",
    "Minus",
    "Times",
    "DividedBy",
    "abs",
    "None"
};
class ExpressionTreeNode {
    private:
        /// @brief 节点储存的值
        double _value=0;
        /// @brief 节点代表的运算
        Operators _operator=Equal;
        /// @brief 节点代表运算所需的参数
        std::vector<ExpressionTreeNode*> _parameters;
        /// @brief 父节点
        ExpressionTreeNode* _father=nullptr;
        /// @brief 表达式
        std::string _expression="";
       std::string _variableName="not";
       std::vector<std::string> _variableNames;
       std::vector<double> _variableValues;
    private:
        /// @brief 括号匹配算法,检测括号是否匹配,去掉多余的最外层一组括号
        /// @return 是否去掉括号,去掉返回true,不去掉或括号不匹配返回false
        bool removeunnecessaryparentheses()
        {
            bool isunnecessary=false;
            if(_expression=="") return false;
            if(_expression[0]!='(') return false;
            int count = 0;
            for(int index=0;index<_expression.length();index++)
            {
                if(_expression[index]=='(')
                {
                    count++;
                }
                else if(_expression[index]==')')
                {
                    if((index==_expression.length()-1)&&count==1)
                    {
                        isunnecessary=true;
                    }
                    count--;
                    if((index<_expression.length()-1)&&count==0)
                    {
                        return false;
                    }
                }
            }
            if(count!=0)
            {
                std::cerr<<"Error:括号不匹配"<<std::endl;
                isunnecessary = false;
            }
            if(isunnecessary)
            {
                _expression.erase(0,1);
                _expression.erase(_expression.length()-1,1);
            }
            return isunnecessary;
        }
        /// @brief 简化表达式字符串并处理开头为正负号的情况
        void easyexpression()
        {
            while(removeunnecessaryparentheses()){};
            if(_expression[0]=='+'||_expression[0]=='-')
            {
                _expression="0"+_expression;
            }
        }
        /// @brief 获取函数运算类型即参数的表达式
        /// @return 参数表达式的vector
        std::vector<std::string> getparameters()
        {
            //std::cout<<_expression<<std::endl;
            int count=0;
            bool getoperator=false;
            std::vector<std::string> parameters_expression;
            if(_expression.length()==0)
            {
                std::cerr<<"Error:出现空字符串)"<<std::endl;
            }
            for(int index=0;index<_expression.length();index++)
            {
                if(_expression[index]=='(')
                {
                    count++;
                }
                if(_expression[index]==')')
                {
                    count--;
                }
                if(_expression[index]=='+'|| _expression[index]=='-')
                {
                    if(count==0)
                    {
                        parameters_expression.push_back(_expression.substr(0,index));
                        parameters_expression.push_back(_expression.substr(index+1));
                        if(_expression[index]=='+')
                        {
                            _operator=Add;
                        }
                        else
                        {
                            _operator=Minus;
                        }
                        getoperator = true;
                        return parameters_expression;
                    }
                }
            }
            for(int index=0;index<_expression.length();index++)
            {
                if(_expression[index]=='(')
                {
                    count++;
                }
                if(_expression[index]==')')
                {
                    count--;
                }
                if(_expression[index]=='*'|| _expression[index]=='/')
                {
                    if(count==0)
                    {
                        parameters_expression.push_back(_expression.substr(0,index));
                        parameters_expression.push_back(_expression.substr(index+1));
                        if(_expression[index]=='*')
                        {
                            _operator=Times;
                        }
                        else
                        {
                            _operator=DividedBy;
                        }
                        getoperator = true;
                        return parameters_expression;
                    }
                }
            }
            if(_expression[0]<='9'&&_expression[0]>='0')
            {
                int pointcount=0;
                bool isfloat=true;
                for(int index=0;index<_expression.length();index++)
                {
                    if(_expression[index]<'0'||+_expression[index]>'9')
                    {
                        if(_expression[index]=='.')
                        {
                            pointcount++;
                        }
                        else isfloat=false;
                    }
                    if(pointcount>1)
                    {
                        std::cerr<<"Error:数字中出现超过一个小数点"<<std::endl;
                        isfloat=false;
                        break;
                    }
                }
                if(isfloat)
                {
                    _value = std::stod(_expression);
                    _operator = Equal;
                    return {};
                }
            }
            if(_expression[0]=='@')
            {
                bool isvariable=false;
                //std::cout<<_expression<<" "<<std::endl;
                for(std::string name: _variableNames)
                {
                    if(_expression.compare(name)==0)
                    {
                        _variableName=_expression;
                        isvariable = true;
                        break;
                    }
                }
                if(isvariable)
                {
                    _operator=Variable;
                    
                    return {};
                }
            }
            if(_expression[0]>='a'&&_expression[0]<='z')
            {
                return cmathfuncparameters();
            }
            _operator=None;
            if(_operator==None)
            {
                std::cerr<<"Error:未定义的运算"<<std::endl;
            }
            return {};
        }
        /// @brief cmath中函数的解析
        /// @return 函数的参数表
        std::vector<std::string> cmathfuncparameters()
        {
            bool isnotoperator=true;
            int end=_expression.find("(");
            if(end==std::string::npos){std::cerr<<"Error:判断为函数但是没有括号"<<std::endl;}
            std::string ope=_expression.substr(0,end);
            for(int index=0;index<SOperators.size();index++)
            {
                if(ope.compare(SOperators[index])==0)
                {
                    _operator=Operators(index);
                    isnotoperator=false;
                }
            }
            if(isnotoperator)
            {
                _operator=None;
                return {};
            }
            std::string sparameters=_expression.substr(end+1,_expression.length()-ope.length()-2);
            int count = 0;
            int comma = -1;
            std::vector<std::string> parameters_expression; 
            for(int index=0;index<sparameters.length();index++)
            {
                if(sparameters[index]=='(')
                {
                    count++;
                }
                if(sparameters[index]==')')
                {
                    count--;
                }
                if(sparameters[index]==',')
                {
                    if(count==0)
                    {
                        int num=index-comma-1;
                        parameters_expression.push_back(sparameters.substr(comma+1,num));
                        comma=index;
                    }
                }
                if(index==sparameters.length()-1)
                {
                    parameters_expression.push_back(sparameters.substr(comma+1));
                }
            }
            return parameters_expression;
        }
        /// @brief 设置参数(子节点)
        void setparameters()
        {
            for(auto parameter_expression:getparameters())
            { 
                _parameters.push_back( new ExpressionTreeNode(parameter_expression,this,_variableNames,_variableValues));
            }
        }

    public:
        ExpressionTreeNode(){}
        ExpressionTreeNode(const std::string& expression,ExpressionTreeNode* father=nullptr,const std::vector<std::string>& variableNames={},const std::vector<double>& variableValues={})
        {
            initialize(expression,father,variableNames,variableValues);
        }
        /// @brief 初始化函数
        /// @param expression 读取的表达式
        /// @param father  父节点的指针
        void initialize(const std::string& expression,ExpressionTreeNode* father=nullptr,const std::vector<std::string>& variableNames={},const std::vector<double>& variableValues={})
        {
            _expression = expression;
            _father = father;
            easyexpression();
            setvariablenames(variableNames);
            setvariablevalues(variableValues);
            setparameters();
        }
        void setvariablenames(const std::vector<std::string> variableNames)
        {
            for(auto name:variableNames)
            {
                _variableNames.push_back(name);
            }
        }
        void setvariablevalues(const std::vector<double> variableValues)
        {
            for(auto value:variableValues)
            {
                _variableValues.push_back(value);
            }
        }
        /// @brief 将运算与Operator枚举匹配,然后根据运算类型对参数进行运算
        /// @return _value值
        double calculate()
        {
            switch (_operator)
            {
            case None:
                std::cerr<<"Error:解析时出现None类型"<<std::endl;
                break;
            case Variable:
            {
                int index=0;
                for(;index<_variableNames.size();index++)
                {
                    if(_variableName.compare(_variableNames[index])==0)
                    break;
                }
                _value=_variableValues[index];
            }
                break;
            case Equal:
                break;
            case Add:
                _value = (*_parameters[0]).calculate()+(*_parameters[1]).calculate();
                break;
            case Minus:
                _value = (*_parameters[0]).calculate()-(*_parameters[1]).calculate();
                break;
            case Times:
                _value = (*_parameters[0]).calculate()*(*_parameters[1]).calculate();
                break;
            case DividedBy:
                _value = (*_parameters[0]).calculate()/(*_parameters[1]).calculate();
                break;
            case Abs:
                _value =abs(_parameters[0]->calculate());
                break;
            default:
                std::cerr<<"无法解析的类型"<<std::endl;
                break;
            }
            return _value;
        }
        /// @brief 外部设置velue值的接口,主要用于对未知数进行赋值
        /// @param value 
        void setvalue(const double& value)
        {
            _value = value;
        }
        /// @brief 获取表达式
        /// @return 表达式
        std::string getexpression()
        {
            return _expression;
        }
        void show()
        {
            std::cout<<std::endl<<"name:";
            for(auto name:_variableNames)
            {
                std::cout<<name<<"  ";
            }
            std::cout<<std::endl;
            std::cout<<"value:";
            for(auto name:_variableValues)
            {
                std::cout<<name<<"  ";
            }
            std::cout<<std::endl;
        }
        void testshow()
        {
            //show();
            if(_operator==Add||_operator==Minus)
            {
                std::cout<<"(";
                _parameters[0]->testshow();
                std::cout<<")+-(";
                _parameters[1]->testshow();
                std::cout<<")";
            }
            else if(_operator==Times||_operator==DividedBy)
            {
                std::cout<<"(";
                _parameters[0]->testshow();
                std::cout<<")*/(";
                _parameters[1]->testshow();
                std::cout<<")";
            }
            else if(_operator==Equal)
            {
                std::cout<<_value;
            }
            else if(_operator==Variable)
            {
                std::cout<<_variableName;
            }
            else if(_operator!=None)
            {
                std::cout<<SOperators[_operator];
                std::cout<<"(";
                _parameters[0]->testshow();
                if(_parameters.size()>1)
                {
                    for(int index=1;index<_parameters.size();index++)
                    {
                        std::cout<<",";
                        _parameters[index]->testshow();
                    }
                }
                std::cout<<")";
            }
        }
        void passvariablevalues(const std::vector<double>& variableValues)
        {
            _variableValues=variableValues;
            for(auto p:_parameters)
            {
                p->passvariablevalues(variableValues);
            }
        }
};
class ExpressionTree {
    private:
        /// @brief 表达树的根节点
        ExpressionTreeNode _rootNode;
    private:
        ///TODO 将函数放到表达式中
    public:
        ExpressionTree(){}
        ExpressionTree(const std::string & expression,const std::vector<std::string>& variableNames={},const std::vector<double>& variableValues={})
        {
            initialize(expression,variableNames,variableValues);
        }
        void initialize(const std::string & expression,const std::vector<std::string>& variableNames={},const std::vector<double>& variableValues={})
        {
            test_trim(expression);
            _rootNode.initialize(expression,nullptr,variableNames,variableValues);
        }
        /// @brief 计算表达数的结果
        /// @return 计算结果
        double calculate()
        {
            return _rootNode.calculate();
        }
        void passvariablevalues(const std::vector<double> & variableValues)
        {
            _rootNode.passvariablevalues(variableValues);
        }
        void test_trim(const std::string& expression)
        {
            int index = 0;
            if(!expression.empty())
            {
                while((index=expression.find(' ',index))!=std::string::npos)
                {
                    std::cerr<<"Error: 表达式中有空格"<<std::endl;
                }
            }
        }
};
class StringFunction {
    private:
        std::string _variableA = "@x";
        std::string _variableB = "@y";
        std::string _function="";
    private:
        /// @brief 
        /// @return 
        double expressiontree()
        {
            return 0;
        }
        /// @brief  去除字符串表达式空格
        void trim()
        {
            int index = 0;
            if(!_function.empty())
            {
                while((index=_function.find(' ',index))!=std::string::npos)
                {
                    _function.erase(index,1);
                }
            }
        }
    public:
        StringFunction(){
            trim();//确保字符串没有空格
        }
};
#endif