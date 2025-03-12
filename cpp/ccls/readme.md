# 操作步骤
1. 项目编译，并生成compile_commands.json文件
cmake . && make

2. 对指定代码进行基于语法的LSP操作
python ccls.py

3. 进行交互式操作
详见ccls.py主函数的命令映射

# reference
lsp: https://microsoft.github.io/language-server-protocol/
ccls github: https://github.com/MaskRay/ccls
cmake tutorial: https://cmake.org/cmake/help/latest/guide/tutorial/index.html
