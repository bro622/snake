#include "Snake.h"
#include <cmath>

Snake::Snake(int startX, int startY) 
    : currentDirection(Direction::RIGHT), nextDirection(Direction::RIGHT),
      moveTimer(0), moveInterval(0.5f), growPending(0) {
    // 初始蛇身长度为 3
    body.push_back(Point(startX, startY));
    body.push_back(Point(startX - 1, startY));
    body.push_back(Point(startX - 2, startY));
}

void Snake::update(float deltaTime) {
    moveTimer += deltaTime;
    
    // 达到移动间隔才更新位置
    if (moveTimer >= moveInterval) {
        moveTimer = 0;
        move();
    }
}

void Snake::move() {
    // 更新实际方向（从缓冲读取）
    currentDirection = nextDirection;
    
    // 计算新头部位置
    Point newHead = getHead();
    switch (currentDirection) {
        case Direction::UP:
            newHead.y -= 1;
            break;
        case Direction::DOWN:
            newHead.y += 1;
            break;
        case Direction::LEFT:
            newHead.x -= 1;
            break;
        case Direction::RIGHT:
            newHead.x += 1;
            break;
        default:
            break;
    }
    
    // 在头部插入新位置
    body.push_front(newHead);
    
    // 如果没有待增长，移除尾部
    if (growPending <= 0) {
        body.pop_back();
    } else {
        growPending--;
    }
}

void Snake::setDirection(Direction dir) {
    // 防止 180 度转向导致自杀
    bool isOpposite = false;
    switch (currentDirection) {
        case Direction::UP:
            isOpposite = (dir == Direction::DOWN);
            break;
        case Direction::DOWN:
            isOpposite = (dir == Direction::UP);
            break;
        case Direction::LEFT:
            isOpposite = (dir == Direction::RIGHT);
            break;
        case Direction::RIGHT:
            isOpposite = (dir == Direction::LEFT);
            break;
        default:
            break;
    }
    
    if (!isOpposite && dir != Direction::NONE) {
        nextDirection = dir;
    }
}

Direction Snake::getDirection() const {
    return currentDirection;
}

void Snake::grow() {
    growPending += 1;
}

bool Snake::checkCollision(int gridWidth, int gridHeight) const {
    Point head = getHead();
    
    // 撞墙检测
    if (head.x < 0 || head.x >= gridWidth || head.y < 0 || head.y >= gridHeight) {
        return true;
    }
    
    return false;
}

bool Snake::checkSelfCollision() const {
    Point head = getHead();
    
    // 从第二个身体部分开始检查（跳过头部）
    for (size_t i = 1; i < body.size(); ++i) {
        if (body[i] == head) {
            return true;
        }
    }
    
    return false;
}

Point Snake::getHead() const {
    return body.front();
}

const std::deque<Point>& Snake::getBody() const {
    return body;
}

size_t Snake::getLength() const {
    return body.size();
}

void Snake::setMoveInterval(float interval) {
    moveInterval = interval;
}

void Snake::reset(int startX, int startY) {
    body.clear();
    body.push_back(Point(startX, startY));
    body.push_back(Point(startX - 1, startY));
    body.push_back(Point(startX - 2, startY));
    currentDirection = Direction::RIGHT;
    nextDirection = Direction::RIGHT;
    moveTimer = 0;
    growPending = 0;
}

void Snake::render(sf::RenderWindow& window, float cellSize) const {
    sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
    
    // 绘制蛇头（深绿色）
    cell.setFillColor(sf::Color(0, 150, 0));
    Point head = getHead();
    cell.setPosition(sf::Vector2f(head.x * cellSize, head.y * cellSize));
    window.draw(cell);
    
    // 绘制蛇身（渐变绿色）
    for (size_t i = 1; i < body.size(); ++i) {
        int intensity = static_cast<int>(100 + 50 * (1.0f - static_cast<float>(i) / body.size()));
        cell.setFillColor(sf::Color(0, intensity, 0));
        cell.setPosition(sf::Vector2f(body[i].x * cellSize, body[i].y * cellSize));
        window.draw(cell);
    }
}

bool Snake::isOpposite(Direction dir1, Direction dir2) {
    return (dir1 == Direction::UP && dir2 == Direction::DOWN) ||
           (dir1 == Direction::DOWN && dir2 == Direction::UP) ||
           (dir1 == Direction::LEFT && dir2 == Direction::RIGHT) ||
           (dir1 == Direction::RIGHT && dir2 == Direction::LEFT);
}
