cmake_minimum_required (VERSION 3.0)	# 指定最低 CMake 版本

project (demo VERSION 1.0)	# 定义项目名称和版本

# 使用 fmt 库的 CMake 配置文件查找该库， REQUIRED 关键字确保在找不到包时生成错误
find_package(fmt CONFIG REQUIRED)

# 创建一个可执行文件
add_executable(main main.cpp)

# 指定可执行文件应链接到 fmt 库，PRIVATE 关键字表明 fmt 仅在生成 main 时需要，不应传播到其他依赖项目。
target_link_libraries(main PRIVATE fmt::fmt)
