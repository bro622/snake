# CMake 配置说明

## 项目结构
```
CMakeSFMLProject/
├── CMakeLists.txt      # CMake 配置文件
├── src/
│   └── main.cpp        # 主程序源码
├── data/               # 游戏资源文件
├── build/              # 构建目录
└── README-CMAKE.md     # 本文件
```

## 快速开始

### 1. 配置和构建项目

#### 使用本地安装的 SFML（推荐）
```bash
# 创建并进入构建目录
mkdir build
cd build

# 配置项目（Windows - MinGW）
cmake .. -G "MinGW Makefiles"

# 或者使用 Visual Studio
cmake .. -G "Visual Studio 17 2022"
```

**注意：** CMakeLists.txt 已配置为优先使用 `C:/SFML-3.0.2` 路径的本地 SFML。
如果你的 SFML 安装在其他位置，请修改 CMakeLists.txt 中的 `CMAKE_PREFIX_PATH`。
# 构建项目
cmake --build .

# 或者使用 cmake 的构建命令
cmake --build . --config Release
```

### 2. 运行程序
```bash
# Windows
bin\snake_game.exe

# Linux/macOS
./bin/snake_game
```

## CMake 配置选项

### 可用选项
- `BUILD_SHARED_LIBS` (默认：OFF)
  - ON: 构建为共享库
  - OFF: 构建为静态库

- `ENABLE_TESTS` (默认：ON)
  - ON: 启用测试
  - OFF: 禁用测试

- `VERBOSE_BUILD` (默认：OFF)
  - ON: 开启详细编译输出
  - OFF: 普通输出

### 使用示例
```bash
# 使用自定义选项配置
cmake .. -DBUILD_SHARED_LIBS=ON -DENABLE_TESTS=OFF -DVERBOSE_BUILD=ON
```

## 主要功能

### 1. SFML 依赖管理

**推荐使用本地安装的 SFML：**
- 项目默认从 `C:/SFML-3.0.2` 查找 SFML
- 如果未找到，会自动从 GitHub 下载（备用方案）
- 支持静态链接和动态链接

**安装 SFML to Windows:**
1. 从 [SFML 官网](https://www.sfml-dev.org/download.php) 下载
2. 解压到 `C:/SFML-3.0.2`
3. 确保包含以下目录：
   - `include/` - 头文件
   - `lib/` - 库文件
   - `bin/` - DLL 文件

**可选：** 如果使用 FetchContent 方式，项目会自动下载 SFML 3.0.1：
```cmake
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS Graphics Window System Audio REQUIRED)
```

### 2. 输出目录结构
```
build/
├── bin/           # 可执行文件
│   ├── snake_game.exe
│   └── data/      # 复制的资源文件
├── lib/           # 库文件
└── _deps/         // FetchContent 下载的依赖
```

### 3. 跨平台支持
- **Windows**: 
  - 自动处理 MinGW/Clang 静态链接
  - 可选择隐藏控制台窗口
  - 支持 NSIS 安装包生成
  
- **Linux/macOS**:
  - 原生支持
  - 生成 TGZ/DEB/RPM 包

### 4. 测试配置
```bash
# 运行测试
ctest

# 详细输出运行测试
ctest -V
```

### 5. 打包配置
```bash
# 生成安装包
cpack

# 生成特定格式的安装包
cpack -G ZIP      # Windows ZIP
cpack -G NSIS     # Windows 安装程序
cpack -G TGZ      # Linux tarball
```

## 常用构建命令

### Debug 版本
```bash
cmake --build . --config Debug
```

### Release 版本
```bash
cmake --build . --config Release
```

### 清理构建
```bash
# 删除整个构建目录
rm -rf build/*

# 或者重新创建
cd ..
rm -rf build
mkdir build
cd build
```

### 查看配置信息
```bash
# 查看 CMake 缓存变量
cmake -L ..

# 查看详细帮助
cmake -LH ..
```

## 安装到系统
```bash
# 安装（需要管理员权限）
cmake --install . --prefix "C:/Program Files/SnakeGame"

# 或者指定用户目录
cmake --install . --prefix "$HOME/.local"
```

## 故障排除

### 问题 1: SFML 下载失败
**解决方案：** 检查网络连接，或手动下载 SFML 后使用本地路径

### 问题 2: 编译错误
**解决方案：** 
- 确保 CMake 版本 >= 3.28
- 确保编译器支持 C++17
- 检查 SFML 依赖是否完整

### 问题 3: 运行时找不到资源文件
**解决方案：** 
- 确保 data 文件夹被正确复制到可执行文件目录
- 检查程序是否从正确的目录运行

## 高级配置

### 更改 SFML 版本
编辑 CMakeLists.txt 中的 GIT_TAG：
```cmake
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1  # 修改为你需要的版本
    ...)
```

### 添加更多源文件
```cmake
# 如果有多个源文件
add_executable(main 
    src/main.cpp
    src/game.cpp
    src/snake.cpp
    # ... 更多文件
)
```

### 自定义编译器标志
```cmake
target_compile_options(main PRIVATE
    -Wall -Wextra -Wpedantic  # GCC/Clang
    # /W4                     # MSVC
)
```

## 更多信息
- [CMake 官方文档](https://cmake.org/documentation/)
- [SFML 官方文档](https://www.sfml-dev.org/documentation/)
- [FetchContent 文档](https://cmake.org/cmake/help/latest/module/FetchContent.html)
