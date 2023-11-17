压缩包中共有以下文件或文件夹，作用说明如下：
D:.
│  Lexical_Analyzer.sln：词法-语法分析器的整体项目，点击进入Visual Studio后可以直接运行。
│  README.txt：本说明文件。
│  编译原理语法分析器实验报告.pdf：实验报告。
│
├─Lexical_Analyzer：程序目录，用于存放项目程序与输入文件。
│      analysis.cpp
│      analysis.h
│      base.cpp
│      base.h
│      code_in.txt：输入源程序文件。
│      grammar.cpp
│      grammar.h
│      grammar.txt：输入文法文件。
│      Lexical_Analyzer.vcxproj
│      Lexical_Analyzer.vcxproj.filters
│      Lexical_Analyzer.vcxproj.user
│      LR1.cpp
│      LR1.h
│      main.cpp
│
└─work_dir：工作目录，用于保存在程序运行过程中输出的文件。
        Analysis_Process.txt：归约过程文件。
        First_Set_Test.txt：First集测试文件。
        Grammar_Rules.txt：语法产生式文件。
        Lexical_Result.txt：词法分析结果文件。
        Lex_to_Parse.txt：词法传入语法分析的单词流文件。
        Parse_Tree.dot：用于生成语法树图片的dot文件。
        Parse_Tree.png：语法分析树图片。
        Pre-Processed_Code.txt：词法分析预处理文件。
        Tables.csv：ACTION表和GOTO表格文件。
        Word_Lable.txt：词法分析中单词种类对应表文件。
        Zero_Closure.txt：0号初始闭包文件。

* 由于在生成语法分析树图片的过程中使用到Graphviz，故如需生成语法分析树图片，需要提前安装Graphviz。
*Graphviz安装说明：
1. 至http://www.graphviz.org/Download_windows.php网站选择系统对应的安装包下载。
2. 打开安装程序进行安装，并配置环境变量（可在安装程序中勾选'Add Graphviz to the system PATH for all users'自动配置环境变量）。
3. 在任意位置打开终端，输入'dot --version'，如果显示Graphviz相关版本信息，则安装成功。