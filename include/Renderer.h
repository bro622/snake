#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Renderer {
private:
    sf::RenderWindow* window;
    float cellSize;       // 格子大小
    int gridWidth;        // 网格宽度（格子数）
    int gridHeight;       // 网格高度（格子数）
    
    // 双缓冲相关 - SFML 自动管理，但我们在逻辑上也分层
    sf::RenderTexture gameBuffer;  // 游戏画面缓冲层
    sf::RenderTexture uiBuffer;    // UI 界面缓冲层
    
public:
    Renderer(sf::RenderWindow* win, int width, int height);
    
    // 初始化渲染器
    bool initialize();
    
    // 清空缓冲区
    void clear();
    
    // 绘制游戏元素到缓冲层
    void drawToGameBuffer(const sf::Drawable& drawable);
    
    // 绘制 UI 文本
    void drawText(const std::string& text, int x, int y, 
                  unsigned int size = 24, sf::Color color = sf::Color::White);
    
    // 交换缓冲（显示到屏幕）
    void display();
    
    // 获取格子大小
    float getCellSize() const;
    
    // 获取网格宽度
    int getGridWidth() const;
    
    // 获取网格高度
    int getGridHeight() const;
    
    // 绘制网格背景（辅助功能）
    void drawGrid() const;
};
