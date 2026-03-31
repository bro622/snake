# 贪吃蛇游戏 - 快速开始指南

## 🚀 立即运行

### 已经编译好的版本
如果已经在 `build_local` 目录构建完成：

```bash
cd build_local
.\bin\snake_game.exe
```

## 🎮 游戏控制

| 按键 | 功能 |
|------|------|
| **W** 或 **↑** | 向上移动 |
| **S** 或 **↓** | 向下移动 |
| **A** 或 **←** | 向左移动 |
| **D** 或 **→** | 向右移动 |
| **P** 或 **Space** | 暂停/继续游戏 |
| **Ctrl + S** | 保存当前游戏进度 |
| **Ctrl + L** | 读取之前保存的游戏 |
| **Enter** 或 **Space** | 开始游戏（在菜单界面）|
| **ESC** | 退出游戏 |

## 📋 功能说明

### 基本玩法
1. 使用方向键控制蛇的移动方向
2. 吃到红色食物得 10 分，蛇身变长
3. 避免撞墙或撞到自己
4. 分数越高，蛇越长，难度越大

### 存档系统
- **自动保存**：游戏结束时会保存最高分
- **手动存档**：游戏中按 `Ctrl+S` 保存当前进度
- **手动读档**：按 `Ctrl+L` 读取上次保存的进度
- **存档位置**：`data/saves/save1.json`

### 游戏状态
- **菜单界面**：显示标题、操作说明、最高分
- **游戏中**：显示当前分数、蛇长度
- **暂停**：按 P 键暂停，再按一次继续
- **游戏结束**：显示最终分数，可重新开始

## 🔧 重新编译项目

### 方法 1：使用 MinGW（推荐）
```bash
cd c:\Users\ASUS\Downloads\ABDM\Compressed\s071cmake-sfml-snake-main
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build . --config Release
.\bin\snake_game.exe
```

### 方法 2：使用 Visual Studio
```bash
cd c:\Users\ASUS\Downloads\ABDM\Compressed\s071cmake-sfml-snake-main
cmake -B build_vs -G "Visual Studio 17 2022"
cmake --build build_vs --config Release
.\build_vs\bin\Release\snake_game.exe
```

## 📁 项目文件说明

```
SnakeGame/
├── CMakeLists.txt          # CMake 配置
├── include/                # 头文件
│   ├── Types.h            # Point, Direction
│   ├── Snake.h            # 蛇类
│   ├── Food.h             # 食物类
│   ├── Input.h            # 输入处理
│   ├── Renderer.h         # 渲染器
│   ├── SaveSystem.h       # 存档系统
│   └── Game.h             # 游戏主类
├── src/                    # 源文件
│   ├── main.cpp           # 主程序
│   ├── Snake.cpp          # 蛇的实现
│   ├── Food.cpp           # 食物的实现
│   ├── Input.cpp          # 输入处理实现
│   ├── Renderer.cpp       # 渲染器实现
│   ├── SaveSystem.cpp     # 存档系统实现
│   └── Game.cpp           # 游戏主逻辑
└── data/                   # 资源文件
    ├── saves/             # 存档目录
    └── ...                # 其他资源
```

## 💡 常见问题

### Q: 游戏无法启动？
**A**: 确保 SFML 3.0.2 安装在 `C:/SFML-3.0.2`，并且所有 DLL 文件已复制到 `build/bin/` 目录。

### Q: 存档文件在哪里？
**A**: `data/saves/save1.json`，可以用文本编辑器打开查看或修改。

### Q: 如何修改游戏速度？
**A**: 编辑 `src/Snake.cpp`，修改构造函数中的 `moveInterval` 值（默认 0.15f）。

### Q: 如何修改窗口大小？
**A**: 编辑 `src/main.cpp`，修改 `sf::VideoMode(sf::Vector2u(800, 600))` 中的尺寸。

### Q: 如何修改网格大小？
**A**: 编辑 `src/main.cpp`，修改 `Game game(&window, 20, 15);` 中的网格参数。

## 🎯 学习目标

通过本项目，你将学习：
- ✅ C++17 现代语法特性
- ✅ SFML 图形库的使用
- ✅ 面向对象程序设计
- ✅ 数据结构（deque）的应用
- ✅ JSON 序列化/反序列化
- ✅ CMake 构建系统
- ✅ 游戏状态机设计
- ✅ 模块化编程实践

## 📚 扩展阅读

- [SFML 官方文档](https://www.sfml-dev.org/documentation/)
- [CMake 官方文档](https://cmake.org/documentation/)
- [C++17 新特性](https://en.cppreference.com/w/cpp/17)
- [JSON 格式规范](https://www.json.org/json-zh.html)

---

**祝你游戏愉快！** 🎮

如有问题，请查看 [PROJECT-REPORT.md](PROJECT-REPORT.md) 获取详细说明。
