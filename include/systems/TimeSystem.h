#pragma once
#include <SFML/System.hpp>

// 游戏速度枚举
enum class GameSpeed {
    EASY,     // 简单：0.2s/格
    NORMAL,   // 普通：0.15s/格
    HARD      // 困难：0.1s/格
};

class TimeSystem {
private:
    mutable sf::Clock clock;  // mutable允许在const方法中修改
    float accumulator;          // 累加器
    float moveInterval;         // 移动间隔
    GameSpeed currentSpeed;
    
public:
    TimeSystem();
    
    // 更新定时器（每帧调用）
    void update(float deltaTime);
    
    // 检查是否应该更新游戏逻辑
    bool shouldUpdate() const;
    
    // 重置累加器（当shouldUpdate返回true后调用）
    void resetAccumulator();
    
    // 速度控制
    void setSpeed(GameSpeed speed);
    GameSpeed getSpeed() const;
    float getMoveInterval() const;
    
    // 获取 deltaTime
    float getDeltaTime() const;
    
private:
    float getIntervalForSpeed(GameSpeed speed) const;
};
