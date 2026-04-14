#include "Renderer.h"

Renderer::Renderer(sf::RenderWindow* win, int width, int height)
    : window(win), gridWidth(width), gridHeight(height) {
    // 计算格子大小（保持正方形）
    float windowSize = std::min(window->getSize().x, window->getSize().y);
    cellSize = windowSize / std::max(width, height);
}

bool Renderer::initialize() {
    // SFML 3.0 RenderTexture 不需要 create，直接使用
    return true;
}

void Renderer::clear() {
    // SFML 3.0: 直接清空窗口
    window->clear(sf::Color(20, 20, 30));
}

void Renderer::drawToGameBuffer(const sf::Drawable& drawable) {
    // 直接绘制到窗口
    window->draw(drawable);
}

void Renderer::drawText(const std::string& text, int x, int y, 
                        unsigned int size, sf::Color color) {
    // SFML 3.0 字体渲染
    static sf::Font font;
    static bool fontLoaded = false;
    static bool loadFailed = false;
    
    if (!fontLoaded && !loadFailed) {
        // 尝试加载字体文件（优先使用宋体，支持中文）
        if (font.openFromFile("data/Fonts/simsun.ttc")) {
            fontLoaded = true;
        } else if (font.openFromFile("data/Fonts/arial.ttf")) {
            fontLoaded = true;
        } else {
            // 如果加载失败，标记不再尝试
            loadFailed = true;
            return;
        }
    }
    
    if (!fontLoaded) {
        return;  // 字体未加载成功
    }
    
    // SFML 3.0: Text 构造函数需要字体
    sf::Text sfText(font, text, size);
    sfText.setFillColor(color);
    sfText.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
    
    window->draw(sfText);
}

void Renderer::display() {
    // SFML 3.0: 显示到屏幕
    window->display();
}

float Renderer::getCellSize() const {
    return cellSize;
}

int Renderer::getGridWidth() const {
    return gridWidth;
}

int Renderer::getGridHeight() const {
    return gridHeight;
}

void Renderer::drawGrid() const {
    // 可选：绘制网格线辅助（简化版本）
}
