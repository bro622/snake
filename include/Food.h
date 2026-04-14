#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include "Types.h"

class Food {
private:
    Point position;
    int scoreValue;  // 食物分值
    bool isEaten;    // 是否被吃掉
    
public:
    Food();
    
    // 生成新食物（不在蛇身上）
    void spawn(const std::deque<Point>& snakeBody, int gridWidth, int gridHeight);
    
    // 获取食物位置
    Point getPosition() const;
    
    // 获取分值
    int getScoreValue() const;
    
    // 标记为被吃
    void setEaten(bool eaten);
    
    // 设置位置（用于读档）
    void setPosition(const Point& pos);
    
    // 检查是否被吃
    bool isEatenBy(const Point& head) const;
    
    // 渲染食物
    void render(sf::RenderWindow& window, float cellSize) const;
};
