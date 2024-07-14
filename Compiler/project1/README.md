# 2022编译Project

### 说明

所有要求请阅读[Project1说明](Project1说明.pdf)

### 环境配置

1. linux，以下以ubuntu为例
2. `sudo apt-get install build-essential`
3. `sudo apt-get install flex`
4. `sudo apt-get install bison`

### DEMO
提供一个表达式词法分析（只有加减法）作为flex入门的参考：[flex_demo](flex_demo)

执行
```
make

./lexer
```
输入表达式，输出表达式词法分析结果

### 参考资料
1. [PCAT语言参考](pcat语言参考指南.pdf)
2. [Flex manual](http://ranger.uta.edu/~fegaras/cse5317/flex/flex_toc.html)
3. [Bison manual](http://ranger.uta.edu/~fegaras/cse5317/bison/bison_toc.html)






