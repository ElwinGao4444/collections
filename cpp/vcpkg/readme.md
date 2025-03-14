# vcpkg官方页面
https://learn.microsoft.com/zh-cn/vcpkg/
https://vcpkg.io/en/packages?query=

# vcpkg的配置与用法
## 下载vcpkg库并配置环境变量
git clone https://github.com/microsoft/vcpkg.git --depth=1
export VCPKG_ROOT=`pwd`/vcpkg

## vcpkg的安装与基本用法
Mac安装vcpkg：brew install vcpkg
搜索：vcpkg search <library>
安装：vcpkg install boost
查看：vcpkg list
更新：vcpkg update
卸载：vcpkg remove boost

# 添加项目文件和依赖项
## 创建清单文件
vcpkg new --application

## 添加依赖项（执行完命令后，vcpkg.json 会发生相应变化）
vcpkg add port fmt
在CMakeLists.txt的target_link_libraries项中链接依赖项

## 配置工具链
构建 CMakePresets.json
* 用于指定项目范围内的构建详细信息。
* 通常包含通用的配置选项，适用于所有开发者。
* 应该被纳入版本控制系统。
构建 CMakeUserPresets.json
* 用于开发者指定自己的本地构建详细信息。
* 通常包含特定于开发者的配置选项。
* 不应该被纳入版本控制系统。

## CMake配置生成 & 项目生成
cmake --preset=default
cmake --build build
./build/main

# 补充说明
1. 对于target_link_libraries中的namespace::lib，具体要如何命名，按照官方文档解析，需要开发者提供明确的usage文件，如果开发者没有提供使用说明，那么就不得不去深挖三方库的构建系统
