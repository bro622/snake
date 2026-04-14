#include "Input.h"

Input::Input(sf::RenderWindow* win) : window(win) {}

Direction Input::processInput() {
    Direction newDirection = Direction::NONE;
    
    std::optional<sf::Event> event;
    while ((event = window->pollEvent()).has_value()) {
        if (event->is<sf::Event::Closed>()) {
            window->close();
        }
        
        if (event->is<sf::Event::KeyPressed>()) {
            auto keyPressed = event->getIf<sf::Event::KeyPressed>();
            if (keyPressed) {
                switch (keyPressed->code) {
                    case sf::Keyboard::Key::Up:
                    case sf::Keyboard::Key::W:
                        newDirection = Direction::UP;
                        break;
                        
                    case sf::Keyboard::Key::Down:
                    case sf::Keyboard::Key::S:
                        newDirection = Direction::DOWN;
                        break;
                        
                    case sf::Keyboard::Key::Left:
                    case sf::Keyboard::Key::A:
                        newDirection = Direction::LEFT;
                        break;
                        
                    case sf::Keyboard::Key::Right:
                    case sf::Keyboard::Key::D:
                        newDirection = Direction::RIGHT;
                        break;
                        
                    default:
                        break;
                }
            }
        }
    }
    
    return newDirection;
}

bool Input::isQuitRequested() const {
    // 检查 ESC 键
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        return true;
    }
    return false;
}

bool Input::isPauseRequested() const {
    // 检查 P 键或空格键
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        return true;
    }
    return false;
}

bool Input::isSaveRequested() const {
    // 检查 S 键（需要配合 Ctrl）
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        return true;
    }
    return false;
}

bool Input::isLoadRequested() const {
    // 检查 L 键（需要配合 Ctrl）
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
        return true;
    }
    return false;
}
