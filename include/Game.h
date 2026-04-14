#pragma once
#include <SFML/Graphics.hpp>
#include "Types.h"
#include "Snake.h"
#include "Food.h"
#include "Renderer.h"
#include "SaveSystem.h"
#include "systems/InputSystem.h"
#include "systems/TimeSystem.h"
#include "core/StateMachine.h"

class Game {
private:
    sf::RenderWindow* window;
    Renderer renderer;
    InputSystem inputSystem;
    TimeSystem timeSystem;
    StateMachine stateMachine;
    SaveSystem saveSystem;
    
    Snake snake;
    Food food;
    
    int score;
    int highScore;  // 最高分
    
    int gridWidth;
    int gridHeight;
    
    // 游戏速度控制（使用GameSpeed替代Difficulty）
    GameSpeed currentSpeed;
    
public:
    Game(sf::RenderWindow* win, int gridW, int gridH);
    
    // 初始化游戏
    bool initialize();
    
    // 运行游戏主循环
    void run();
    
    // 停止游戏
    void stop();
    
    // 设置速度
    void setGameSpeed(GameSpeed speed);
    
    // 获取速度字符串
    std::string getSpeedString() const;
    
private:
    // 标准游戏循环：processInput -> update -> render
    void processInput();
    void update(float deltaTime);
    void render();
    
    // 重置游戏
    void resetGame();
    
    // 检查碰撞和得分
    void checkCollisions();
    
    // 生成食物
    void spawnFood();
    
    // 更新分数
    void updateScore(int points);
    
    // 保存游戏
    void saveGame();
    
    // 读取游戏
    void loadGame();
    
    // 渲染菜单
    void renderMenu();
    
    // 渲染暂停界面
    void renderPause();
    
    // 渲染游戏结束界面
    void renderGameOver();
    
    // 渲染 UI（分数等）
    void renderUI();
    
    // 获取速度对应的移动间隔
    float getMoveIntervalForSpeed() const;
};
