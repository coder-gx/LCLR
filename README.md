# LCLR——Light Compiler based on LR(1) Method

## 简介

同济大学编译原理课程作业，目前只实现到词法分析和语法分析阶段，今后会继续更新...

采用LR(1)分析方法，采用C++实现后端编译，采用Qt实现前端界面


项目成员：

[**coder-gx**](https://github.com/coder-gx)

[**WinstonLiyt**](https://github.com/WinstonLiyt)

[**CoffeeMark2**](https://github.com/CoffeeMark2) 

## 运行

1.将项目clone到本地，可以在Visual Studio 2022下可以直接运行

2.由于在生成语法分析树图片的过程中使用到Graphviz，故如需生成语法分析树图片，需要提前安装Graphviz。
Graphviz安装说明：

1. 至http://www.graphviz.org/Download_windows.php网站选择系统对应的安装包下载。
2. 打开安装程序进行安装，并配置环境变量（可在安装程序中勾选'Add Graphviz to the system PATH for all users'自动配置环境变量）。
3. 在任意位置打开终端，输入`dot --version`，如果显示Graphviz相关版本信息，则安装成功。

## 演示

1.开始界面

<img src="https://github.com/coder-gx/LCLR/assets/105330548/9d9c6839-3547-49d0-941e-c786453cd4a3" alt="image" style="width: 50%; height: 50%;">




2.点击文件，导入源代码并且导入文法，此外源代码可以自己在文本框输入

<img src="https://github.com/coder-gx/LCLR/assets/105330548/0de10f06-0b77-40ae-9230-4a309341b94e" alt="image1" style="width: 50%; height: 50%;">

<img src="https://github.com/coder-gx/LCLR/assets/105330548/04f46684-000b-47e6-b1a6-729d4f1a6e93" alt="image2" style="width: 50%; height: 50%;">




3.点击编译->开始编译，可以选择是否继续生成语法DFA

<img src="https://github.com/coder-gx/LCLR/assets/105330548/ee43e576-b511-4dbb-8e7c-40c6f02cfe14" alt="image" style="width: 50%; height: 50%;">



4.可以进行错误定位

<img src="https://github.com/coder-gx/LCLR/assets/105330548/e2096712-747c-4a9a-b78c-be057c32e500" alt="image1" style="width: 50%; height: 50%;">
<img src="https://github.com/coder-gx/LCLR/assets/105330548/332e35a4-bdca-44ea-8bd8-258c7f3e8156" alt="image2" style="width: 50%; height: 50%;">



