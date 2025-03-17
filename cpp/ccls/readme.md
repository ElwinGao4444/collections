# 操作步骤
1. 项目编译，并生成compile_commands.json文件
cmake . && make

2. 对指定代码进行基于语法的LSP操作
brew install ccls
pip install python-lsp-jsonrpc
python ccls.py

3. 进行交互式操作
o main.cpp          # 打开指定文件
c main.cpp          # 关闭指定文件
d main.cpp 33 17    # 根据行号/列号查找defination
r main.cpp 33 17    # 根据行号/列号查找reference

# reference
lsp: https://microsoft.github.io/language-server-protocol/
ccls github: https://github.com/MaskRay/ccls
cmake tutorial: https://cmake.org/cmake/help/latest/guide/tutorial/index.html
