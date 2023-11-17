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

<img src="C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20231117191409669.png" alt="image-20231117191409669" style="zoom: 67%;" />

2.点击文件，导入源代码并且导入文法，此外源代码可以自己在文本框输入

![image-20231117191755853](C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20231117191755853.png)<img src="C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20231117191953947.png" alt="image-20231117191953947" style="zoom: 50%;" />

3.点击编译->开始编译，可以选择是否继续生成语法DFA

<img src="C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20231117192130417.png" alt="image-20231117192130417" style="zoom: 67%;" />

4.可以进行错误定位

<img src="C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20231117192644430.png" alt="image-20231117192644430" style="zoom: 50%;" /><img src="C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20231117192804189.png" alt="image-20231117192804189" style="zoom:50%;" />

