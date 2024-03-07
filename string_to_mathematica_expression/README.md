# 使用

支持将带有cmath函数以及未知变量的数学表达式字符串解析为表达树并可以进行计算;

目前仅支持加减乘除\括号\正负号\部分cmath库函数\未知数组成的数学表达式的解析;

变量请用@开头;

只需简单的修改代码即可增加函数支持;

## 初始化

`ExpressionTree tree{表达式字符串,变量名vector,变量值vector};`

## 计算

`tree.caculate()->double;`

## 修改参数

`tree.passvariablevalues(const std::vector<double>& values)`

由于设计时考虑不周密,目前修改参数需要遍历整棵表达树,考虑有空时将Node和Tree中对于value的储存改为指针,通过更改一块数据区域来完成参数传递;

## 增加函数支持

1. 在`string_function.h`最上方的枚举`Operators`和全局常量`SOperators`中,分别添加枚举名称和对应的函数名字符串,注意应该有相同的次序;
2. 在`ExpressionTreeNode`的`calculate`函数的`swich`语句中增加函数返回值的逻辑;
3. `_parameters`为函数参数的表达式转化成的Node,`_value`为返回值;
