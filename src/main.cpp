#include <SFML/Graphics.hpp>
#include "Game.h"

int main() {
    // 创建窗口 800x600
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Snake Game - SFML", sf::Style::Close);
    window.setFramerateLimit(60);  // 限制帧率
    
    // 创建游戏实例（网格 20x15）
    Game game(&window, 20, 15);
    
    // 初始化游戏
    if (!game.initialize()) {
        return -1;
    }
    
    // 运行游戏主循环（标准结构：processInput -> update -> render）
    game.run();
    
    return 0;
}
