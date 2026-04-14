#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Types.h"

class Input {
private:
    sf::RenderWindow* window;
    
public:
    Input(sf::RenderWindow* win);
    
    // 处理输入，返回方向改变
    Direction processInput();
    
    // 检查是否请求退出
    bool isQuitRequested() const;
    
    // 检查是否请求暂停
    bool isPauseRequested() const;
    
    // 检查是否请求存档
    bool isSaveRequested() const;
    
    // 检查是否请求读档
    bool isLoadRequested() const;
};
