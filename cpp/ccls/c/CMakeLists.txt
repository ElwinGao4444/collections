# 子模块的配置

# 子模块的源文件
set(LIB_SOURCE_CODE c.cpp)

# 子模块的库
add_library(CLib ${LIB_SOURCE_CODE})

# 子模块的头文件路径
target_include_directories(CLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
