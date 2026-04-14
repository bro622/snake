#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include "Types.h"

class Snake {
private:
    std::deque<Point> body;  // 使用 deque 管理蛇身 - 双端队列便于头部插入尾部删除
    Direction currentDirection;
    Direction nextDirection;  // 缓冲下一次方向，避免快速按键导致自杀
    float moveTimer;
    float moveInterval;  // 移动间隔，控制速度
    int growPending;     // 待增长长度

public:
    Snake(int startX, int startY);
    
    // 更新蛇的位置
    void update(float deltaTime);
    
    // 移动蛇（由外部时间系统控制频率）
    void move();
    
    // 改变方向
    void setDirection(Direction dir);
    
    // 获取当前方向
    Direction getDirection() const;
    
    // 吃食物，准备增长
    void grow();
    
    // 碰撞检测
    bool checkCollision(int gridWidth, int gridHeight) const;
    
    // 是否撞到自身
    bool checkSelfCollision() const;
    
    // 获取蛇头位置
    Point getHead() const;
    
    // 获取蛇身
    const std::deque<Point>& getBody() const;
    
    // 获取蛇的长度
    size_t getLength() const;
    
    // 设置移动间隔（难度控制）
    void setMoveInterval(float interval);
    
    // 重置蛇
    void reset(int startX, int startY);
    
    // 渲染蛇
    void render(sf::RenderWindow& window, float cellSize) const;
    
    // 静态辅助函数：判断两个方向是否相反
    static bool isOpposite(Direction dir1, Direction dir2);
};
