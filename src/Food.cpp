#include "Food.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

Food::Food() : scoreValue(10), isEaten(false) {
    // 随机种子在 Game 类中初始化
}

void Food::spawn(const std::deque<Point>& snakeBody, int gridWidth, int gridHeight) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    Point newPos;
    bool validPosition = false;
    
    // 生成不在蛇身上的位置
    while (!validPosition) {
        std::uniform_int_distribution<> distX(0, gridWidth - 1);
        std::uniform_int_distribution<> distY(0, gridHeight - 1);
        
        newPos.x = distX(gen);
        newPos.y = distY(gen);
        
        // 检查是否在蛇身上
        validPosition = true;
        for (const auto& segment : snakeBody) {
            if (segment == newPos) {
                validPosition = false;
                break;
            }
        }
    }
    
    position = newPos;
    isEaten = false;
}

Point Food::getPosition() const {
    return position;
}

int Food::getScoreValue() const {
    return scoreValue;
}

void Food::setEaten(bool eaten) {
    isEaten = eaten;
}

void Food::setPosition(const Point& pos) {
    position = pos;
    isEaten = false;
}

bool Food::isEatenBy(const Point& head) const {
    return !isEaten && (head == position);
}

void Food::render(sf::RenderWindow& window, float cellSize) const {
    sf::CircleShape shape(cellSize / 2.0f - 2);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(sf::Vector2f(position.x * cellSize + 1, position.y * cellSize + 1));
    window.draw(shape);
}
