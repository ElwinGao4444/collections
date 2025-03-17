# 操作步骤
1. 项目编译，并生成compile_commands.json文件
cmake . && make

2. 对指定代码进行基于语法的LSP操作
brew install ccls
pip install python-lsp-jsonrpc
python ccls.py

3. 进行交互式操作
o main.cpp          # 打开指定文件
d main.cpp 33 17    # 根据行号/列号查找defination
c main.cpp          # 关闭指定文件
o a/a.cpp           # 打开指定文件
r a/a.cpp 20 4      # 根据行号/列号查找reference
c a/a.cpp           # 关闭指定文件

# reference
lsp: https://microsoft.github.io/language-server-protocol/
ccls github: https://github.com/MaskRay/ccls
cmake tutorial: https://cmake.org/cmake/help/latest/guide/tutorial/index.html
