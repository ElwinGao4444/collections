cmake_minimum_required (VERSION 3.0)	# 指定最低 CMake 版本

project (demo VERSION 1.0)	# 定义项目名称和版本

# 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件搜索路径
include_directories (a b)

# 添加源文件
aux_source_directory (a AObj)	# 方式一：自动收集指定目录下的所有源文件
add_library(BLib b/b.cpp)		# 方式二：在顶层直接创建一个链接库 BLib
add_subdirectory(c)				# 方式三：子目录自行组织生成链接库 CLib

set(MAIN main.cpp)	# 定义变量

# 创建一个可执行文件
add_executable(main ${MAIN} ${AObj})

# 链接库到可执行文件
target_link_libraries(main BLib CLib)

