#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Types.h"
#include <queue>

class InputSystem {
private:
    sf::RenderWindow* window;
    
    // 输入队列和最新合法输入
    std::queue<Direction> inputQueue;
    Direction latestValidInput = Direction::NONE;
    
    // 功能键状态
    bool quitRequested = false;
    bool pauseRequested = false;
    bool saveRequested = false;
    bool loadRequested = false;
    bool startRequested = false;
    
    // 难度切换
    bool difficultyEasy = false;
    bool difficultyNormal = false;
    bool difficultyHard = false;
    
public:
    InputSystem(sf::RenderWindow* win);
    
    // 核心：获取最新合法方向（带方向约束）
    Direction getLatestValidDirection(Direction currentDir);
    
    // 方向约束：判断两个方向是否相反
    static bool isOpposite(Direction dir1, Direction dir2);
    
    // 功能键查询
    bool isQuitRequested() const { return quitRequested; }
    bool isPauseRequested() const { return pauseRequested; }
    bool isSaveRequested() const { return saveRequested; }
    bool isLoadRequested() const { return loadRequested; }
    bool isStartRequested() const { return startRequested; }
    
    // 难度切换查询
    bool isDifficultyEasy() const { return difficultyEasy; }
    bool isDifficultyNormal() const { return difficultyNormal; }
    bool isDifficultyHard() const { return difficultyHard; }
    
    // 重置功能键状态（每帧调用）
    void resetFrameState();
    
private:
    // 处理事件队列
    void processEvents();
    
    // 验证并存储输入
    void validateAndStoreInput(Direction input, Direction currentDir);
    
    // 键盘码转换为方向
    Direction keyToDirection(sf::Keyboard::Key key);
};
