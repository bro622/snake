# 贪吃蛇游戏项目 - 工程化实现报告

## 📋 项目概述

本项目是一个完整的、模块化的贪吃蛇游戏，使用 C++17 和 SFML 3.0.2 开发，实现了完整的游戏功能和存档系统。

### 项目结构
```
SnakeGame/
├── CMakeLists.txt              # CMake 配置文件
├── include/                    # 头文件目录
│   ├── Types.h                # 基础数据结构（Point, Direction）
│   ├── Snake.h                # 蛇类定义
│   ├── Food.h                 # 食物类定义
│   ├── Input.h                # 输入处理类定义
│   ├── Renderer.h             # 渲染器类定义
│   ├── SaveSystem.h           # 存档系统定义
│   └── Game.h                 # 游戏主类定义
├── src/                        # 源文件目录
│   ├── main.cpp               # 主程序入口
│   ├── Snake.cpp              # 蛇类实现
│   ├── Food.cpp               # 食物类实现
│   ├── Input.cpp              # 输入处理实现
│   ├── Renderer.cpp           # 渲染器实现
│   ├── SaveSystem.cpp         # 存档系统实现
│   └── Game.cpp               # 游戏主逻辑实现
└── data/                       # 游戏资源
    ├── saves/                 # 存档文件目录
    ├── Fonts/                 # 字体文件
    └── images/                # 图片资源
```

## ✅ 功能完成情况

### 1. 基础功能（60%）✅

#### （1）控制台界面渲染（双缓冲）
- **实现方式**：使用 SFML 的 RenderWindow 自动双缓冲
- **技术细节**：
  ```cpp
  // SFML 内部自动管理双缓冲
  window->clear(sf::Color(20, 20, 30));  // 清空后台缓冲
  // ... 绘制游戏元素 ...
  window->display();  // 交换前后缓冲
  ```
- **效果**：无闪烁、流畅的图形渲染

#### （2）贪吃蛇基本机制
- ✅ **移动**：使用 `std::deque` 管理蛇身，头部插入新位置，尾部移除
- ✅ **吃食物增长**：碰撞检测 + 延迟增长机制
- ✅ **碰撞检测**：
  - 墙体碰撞：检查蛇头坐标是否超出网格范围
  - 自身碰撞：遍历蛇身检查是否与头部重叠
- ✅ **键盘输入控制**：
  - WASD 或方向键控制方向
  - ESC 退出游戏
  - 防止 180 度转向自杀

#### （3）分数系统
- 每个食物 +10 分
- 实时显示当前分数
- 最高分持久化保存

### 2. 进阶功能（30%）✅

#### （1）存档/读档系统（核心要求）✅

**存储的数据**：
```cpp
struct SaveData {
    std::deque<Point> snake;      // 蛇身位置
    Point foodPosition;            // 食物位置
    Direction direction;           // 当前方向
    int score;                     // 分数
    int gridWidth;                 // 网格宽度
    int gridHeight;                // 网格高度
};
```

**存储格式**：JSON 文本格式（`.json` 文件）
```json
{
  "score": 50,
  "direction": 1,
  "gridWidth": 20,
  "gridHeight": 15,
  "snake": [[10,7],[9,7],[8,7]],
  "food": [15,10]
}
```

**序列化实现**：
```cpp
// 序列化
std::string SaveSystem::serializeToJson(const SaveData& data) const {
    std::stringstream ss;
    ss << "{\n";
    ss << "  \"score\": " << data.score << ",\n";
    ss << "  \"snake\": [";
    for (size_t i = 0; i < data.snake.size(); ++i) {
        ss << "[" << data.snake[i].x << "," << data.snake[i].y << "]";
        // ... 
    }
    ss << "],\n";
    ss << "  \"food\": [" << data.foodPosition.x << "," << data.foodPosition.y << "]\n";
    ss << "}";
    return ss.str();
}

// 反序列化
bool SaveSystem::deserializeFromJson(const std::string& json, SaveData& data) const {
    // 解析 JSON 字符串，提取各个字段
    // 使用 string::find 和 stoi 进行手动解析
}
```

**存档操作**：
- **手动存档**：`Ctrl + S`
- **手动读档**：`Ctrl + L`
- **自动保存最高分**：游戏结束时

#### （2）游戏状态管理 ✅

**状态机设计**：
```cpp
enum class GameState {
    MENU,      // 主菜单
    PLAYING,   // 游戏中
    PAUSED,    // 暂停
    GAMEOVER,  // 游戏结束
    SAVING,    // 存档中
    LOADING    // 读档中
};
```

**状态转换**：
- MENU → PLAYING：按 Enter 或 Space
- PLAYING ↔ PAUSED：按 P 或 Space
- PLAYING → GAMEOVER：碰撞检测失败
- GAMEOVER → PLAYING：按 Enter 或 Space（重新开始）

### 3. 工程规范（10%）✅

#### 模块化设计
完全符合要求的目录结构：
```
├── main.cpp          ✅
├── Game.h/.cpp       ✅
├── Snake.h/.cpp      ✅
├── Food.h/.cpp       ✅
├── Renderer.h/.cpp   ✅
├── Input.h/.cpp      ✅
└── SaveSystem.h/.cpp ✅
```

#### CMake 工程配置
```cmake
# 收集所有源文件
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Snake.cpp
    src/Food.cpp
    src/Input.cpp
    src/Renderer.cpp
    src/SaveSystem.cpp
)

# 包含头文件目录
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 链接 SFML 库
target_link_libraries(main PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)
```

## 🎮 游戏控制

| 按键 | 功能 |
|------|------|
| W / ↑ | 向上移动 |
| S / ↓ | 向下移动 |
| A / ← | 向左移动 |
| D / → | 向右移动 |
| P / Space | 暂停/继续 |
| Ctrl + S | 保存游戏 |
| Ctrl + L | 读取游戏 |
| Enter / Space | 开始游戏（菜单中）|
| ESC | 退出游戏 |

## 🔧 编译与运行

### 环境要求
- C++17 兼容编译器（GCC 14+, MSVC 2022+）
- CMake 3.28+
- SFML 3.0.2
- Windows 10/11

### 编译步骤
```bash
# 1. 创建并进入构建目录
mkdir build
cd build

# 2. 配置项目
cmake .. -G "MinGW Makefiles"
# 或 cmake .. -G "Visual Studio 17 2022"

# 3. 构建项目
cmake --build . --config Release

# 4. 运行游戏
.\bin\snake_game.exe
```

## 💾 存档示例

### 存档文件位置
`data/saves/save1.json`

### 存档文件示例
```json
{
  "score": 120,
  "direction": 2,
  "gridWidth": 20,
  "gridHeight": 15,
  "snake": [[10,7],[9,7],[8,7],[7,7],[6,7]],
  "food": [18,5]
}
```

## 🤖 AI 使用情况说明

### 使用的 AI 工具
- **GitHub Copilot**：代码补全、函数生成
- **ChatGPT/Qoder**：架构设计、调试建议

### AI 生成的关键代码片段

#### 1. JSON 序列化/反序列化（经修改）
**AI 生成原始版本**：
```cpp
// 简单的键值对输出
ss << "\"score\":" << data.score;
```

**修改后版本**：
```cpp
// 添加格式化、数组支持、错误处理
ss << "{\n";
ss << "  \"score\": " << data.score << ",\n";
// ... 完整的 JSON 结构
```

#### 2. 蛇身碰撞检测（经优化）
**AI 初始版本**：
```cpp
for (auto segment : body) {
    if (segment == head) return true;
}
```

**修改后版本**：
```cpp
// 跳过头部检查（避免误判）
for (size_t i = 1; i < body.size(); ++i) {
    if (body[i] == head) return true;
}
```

### 遇到的错误与调试过程

#### 错误 1: SFML 3.0 API 变化
**问题**：SFML 3.0 的 Event 系统完全重构
```cpp
// SFML 2.x 写法（错误）
sf::Event event;
while (window->pollEvent(event)) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) { ... }
    }
}

// SFML 3.0 正确写法
std::optional<sf::Event> event;
while ((event = window->pollEvent()).has_value()) {
    if (event->is<sf::Event::KeyPressed>()) {
        auto keyPressed = event->getIf<sf::Event::KeyPressed>();
        if (keyPressed) {
            switch (keyPressed->code) { ... }
        }
    }
}
```

**调试过程**：
1. 编译报错：`no member named 'type' in 'sf::Event'`
2. 查阅 SFML 3.0 文档发现 API 变化
3. 使用 `std::optional` 和类型安全的 `getIf<>` 方法

#### 错误 2: setPosition 参数变化
**问题**：SFML 3.0 不再接受两个 float 参数
```cpp
// 错误写法
shape.setPosition(x * cellSize, y * cellSize);

// 正确写法
shape.setPosition(sf::Vector2f(x * cellSize, y * cellSize));
```

#### 错误 3: 头文件缺失
**问题**：`std::deque`, `std::random` 等未包含
**解决**：在所有相关头文件中添加必要的 include

## 📊 数据结构选型分析

### 为什么选择 `std::deque` 而不是链表？

#### `std::deque` 优势
1. **头部插入/删除高效**：O(1) 时间复杂度
2. **尾部访问高效**：O(1) 时间复杂度
3. **随机访问支持**：O(1) 时间复杂度（便于碰撞检测）
4. **内存局部性好**：比链表更好的缓存性能

#### 链表劣势
1. **随机访问慢**：需要 O(n) 遍历
2. **额外内存开销**：每个节点需要存储指针
3. **缓存不友好**：内存不连续

#### 蛇的移动算法
```cpp
void Snake::update(float deltaTime) {
    // 计算新头部位置
    Point newHead = getHead();
    // ... 根据方向更新坐标 ...
    
    body.push_front(newHead);  // 头部插入新位置 - O(1)
    
    if (growPending <= 0) {
        body.pop_back();  // 尾部移除 - O(1)
    } else {
        growPending--;
    }
}
```

## 🎯 技术亮点

### 1. 方向缓冲机制
防止快速按键导致蛇 180 度转向自杀：
```cpp
Direction nextDirection;  // 缓冲下一次方向

void Snake::setDirection(Direction dir) {
    // 只在安全时更新缓冲
    if (!isOpposite && dir != Direction::NONE) {
        nextDirection = dir;
    }
}

void Snake::update(float deltaTime) {
    if (moveTimer >= moveInterval) {
        currentDirection = nextDirection;  // 实际移动时才应用
        // ...
    }
}
```

### 2. 延迟增长机制
```cpp
int growPending;  // 待增长长度

void Snake::grow() {
    growPending += 1;  // 不是立即增长
}

void Snake::update(float deltaTime) {
    // ...
    if (growPending <= 0) {
        body.pop_back();  // 正常移除尾部
    } else {
        growPending--;  // 保留尾部（增长）
    }
}
```

### 3. 安全的存档恢复
```cpp
void Game::loadGame() {
    SaveData data;
    if (saveSystem.loadGame(data, "save1")) {
        // 验证网格尺寸兼容性
        if (data.gridWidth == gridWidth && data.gridHeight == gridHeight) {
            // 恢复蛇身
            auto& body = const_cast<std::deque<Point>&>(snake.getBody());
            body = data.snake;
            // ... 恢复其他状态 ...
        } else {
            std::cout << "Save file incompatible (grid size mismatch)" << std::endl;
        }
    }
}
```

## 📝 总结

本项目成功实现了：
- ✅ 完整的贪吃蛇游戏功能
- ✅ 模块化、工程化的代码组织
- ✅ JSON 格式的存档/读档系统
- ✅ 清晰的游戏状态管理
- ✅ 使用现代 C++ 和 SFML 3.0 特性
- ✅ 符合所有作业要求

项目展示了：
1. **系统整合能力**：渲染/输入/数据结构/游戏逻辑的模块化
2. **数据结构应用**：使用 `std::deque` 高效管理蛇身
3. **工程能力**：CMake 构建、模块化设计
4. **AI 辅助开发**：合理使用 AI 工具并进行人工审核和优化
5. **状态持久化**：完整的序列化/反序列化实现

---

**项目名称**：Snake Game - SFML Edition  
**开发时间**：2026 年 3 月  
**技术栈**：C++17, SFML 3.0.2, CMake  
**作者**：学生姓名  
**AI 协助**：GitHub Copilot, ChatGPT/Qoder
