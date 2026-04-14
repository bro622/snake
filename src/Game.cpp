#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow* win, int gridW, int gridH)
    : window(win), renderer(win, gridW, gridH), 
      inputSystem(win), timeSystem(), stateMachine(),
      snake(gridW / 2, gridH / 2), food(),
      score(0), highScore(0), gridWidth(gridW), gridHeight(gridH),
      currentSpeed(GameSpeed::NORMAL) {
}

bool Game::initialize() {
    // 初始化渲染器
    if (!renderer.initialize()) {
        return false;
    }
    
    // 初始化存档系统
    saveSystem.initialize("data/saves");
    
    // 加载最高分
    SaveData dummyData;
    if (saveSystem.loadGame(dummyData, "highscore")) {
        highScore = dummyData.score;
    }
    
    // 初始生成食物
    spawnFood();
    
    // 设置初始速度
    timeSystem.setSpeed(currentSpeed);
    snake.setMoveInterval(getMoveIntervalForSpeed());
    
    return true;
}

void Game::run() {
    // 标准游戏循环
    while (window->isOpen()) {
        // 1. 处理输入（高频）
        processInput();
        
        // 检查是否请求退出
        if (stateMachine.getCurrentState() == GameState::EXIT) {
            break;
        }
        
        // 2. 更新游戏逻辑（每帧都更新，Snake内部控制速度）
        float deltaTime = timeSystem.getDeltaTime();
        update(deltaTime);
        
        // 3. 渲染输出（每帧）
        render();
    }
}

void Game::stop() {
    window->close();
}

// ===== 输入处理 =====
void Game::processInput() {
    // 获取最新合法方向（带180度反转保护）
    Direction newDirection = inputSystem.getLatestValidDirection(snake.getDirection());
    
    // 只在PLAYING状态下应用方向
    if (stateMachine.isPlaying() && newDirection != snake.getDirection()) {
        snake.setDirection(newDirection);
    }
    
    // 处理退出
    if (inputSystem.isQuitRequested()) {
        stateMachine.setState(GameState::EXIT);
        stop();
        return;
    }
    
    // 处理状态转换
    switch (stateMachine.getCurrentState()) {
        case GameState::MENU:
            if (inputSystem.isStartRequested()) {
                resetGame();
                stateMachine.setState(GameState::PLAYING);
            }
            break;
            
        case GameState::PLAYING:
            if (inputSystem.isPauseRequested()) {
                stateMachine.setState(GameState::PAUSED);
            }
            break;
            
        case GameState::PAUSED:
            if (inputSystem.isPauseRequested()) {
                stateMachine.setState(GameState::PLAYING);
            }
            break;
            
        case GameState::GAMEOVER:
            if (inputSystem.isStartRequested()) {
                resetGame();
                stateMachine.setState(GameState::PLAYING);
            }
            break;
            
        default:
            break;
    }
    
    // 处理存档/读档（仅PLAYING状态）
    if (stateMachine.isPlaying()) {
        if (inputSystem.isSaveRequested()) {
            saveGame();
        }
        if (inputSystem.isLoadRequested()) {
            loadGame();
        }
    }
    
    // 处理速度切换（MENU或PLAYING状态）
    if (stateMachine.isMenu() || stateMachine.isPlaying()) {
        if (inputSystem.isDifficultyEasy()) {
            setGameSpeed(GameSpeed::EASY);
        } else if (inputSystem.isDifficultyNormal()) {
            setGameSpeed(GameSpeed::NORMAL);
        } else if (inputSystem.isDifficultyHard()) {
            setGameSpeed(GameSpeed::HARD);
        }
    }
}

// ===== 游戏逻辑更新 =====
void Game::update(float deltaTime) {
    // 只在PLAYING状态下更新游戏逻辑
    if (!stateMachine.isPlaying()) {
        return;
    }
    
    // 更新蛇的位置（传入deltaTime让Snake内部控制速度）
    snake.update(deltaTime);
    
    // 检查碰撞
    checkCollisions();
}

// ===== 渲染 =====
void Game::render() {
    renderer.clear();
    
    switch (stateMachine.getCurrentState()) {
        case GameState::MENU:
            renderMenu();
            break;
            
        case GameState::PLAYING:
            snake.render(*window, renderer.getCellSize());
            food.render(*window, renderer.getCellSize());
            renderUI();
            break;
            
        case GameState::PAUSED:
            snake.render(*window, renderer.getCellSize());
            food.render(*window, renderer.getCellSize());
            renderUI();
            renderPause();
            break;
            
        case GameState::GAMEOVER:
            snake.render(*window, renderer.getCellSize());
            food.render(*window, renderer.getCellSize());
            renderUI();
            renderGameOver();
            break;
            
        default:
            break;
    }
    
    renderer.display();
}

void Game::resetGame() {
    snake.reset(gridWidth / 2, gridHeight / 2);
    score = 0;
    spawnFood();
    timeSystem.setSpeed(currentSpeed);
    snake.setMoveInterval(getMoveIntervalForSpeed());
}

void Game::checkCollisions() {
    Point head = snake.getHead();
    
    // 检查是否吃到食物
    if (food.isEatenBy(head)) {
        snake.grow();
        updateScore(food.getScoreValue());
        spawnFood();
    }
    
    // 检查撞墙
    if (snake.checkCollision(gridWidth, gridHeight)) {
        stateMachine.setState(GameState::GAMEOVER);
        
        // 更新最高分
        if (score > highScore) {
            highScore = score;
            SaveData hsData;
            hsData.score = highScore;
            saveSystem.saveGame(hsData, "highscore");
        }
        return;
    }
    
    // 检查撞自己
    if (snake.checkSelfCollision()) {
        stateMachine.setState(GameState::GAMEOVER);
        
        if (score > highScore) {
            highScore = score;
            SaveData hsData;
            hsData.score = highScore;
            saveSystem.saveGame(hsData, "highscore");
        }
        return;
    }
}

void Game::spawnFood() {
    food.spawn(snake.getBody(), gridWidth, gridHeight);
}

void Game::updateScore(int points) {
    score += points;
}

void Game::saveGame() {
    SaveData data;
    data.snake = snake.getBody();
    data.foodPosition = food.getPosition();
    data.direction = snake.getDirection();
    data.score = score;
    data.gridWidth = gridWidth;
    data.gridHeight = gridHeight;
    
    if (saveSystem.saveGame(data, "save1")) {
        std::cout << "Game saved!" << std::endl;
    } else {
        std::cout << "Failed to save game!" << std::endl;
    }
}

void Game::loadGame() {
    SaveData data;
    
    if (saveSystem.loadGame(data, "save1")) {
        // 恢复游戏状态
        snake.reset(0, 0);  // 先重置
        
        // 恢复蛇身
        auto& body = const_cast<std::deque<Point>&>(snake.getBody());
        body = data.snake;
        
        // 恢复其他状态
        food.setPosition(data.foodPosition);
        score = data.score;
        
        // 如果网格尺寸匹配，更新
        if (data.gridWidth == gridWidth && data.gridHeight == gridHeight) {
            stateMachine.setState(GameState::PLAYING);
            std::cout << "Game loaded!" << std::endl;
        } else {
            std::cout << "Save file incompatible (grid size mismatch)" << std::endl;
        }
    } else {
        std::cout << "No save file found!" << std::endl;
    }
}

void Game::renderMenu() {
    renderer.drawText("SNAKE GAME", 200, 100, 48, sf::Color::Green);
    renderer.drawText("Press ENTER or SPACE to start", 150, 200, 24, sf::Color::White);
    renderer.drawText("Controls:", 200, 300, 24, sf::Color::Yellow);
    renderer.drawText("WASD or Arrow Keys - Move", 200, 340, 18, sf::Color::White);
    renderer.drawText("P or SPACE - Pause", 200, 370, 18, sf::Color::White);
    renderer.drawText("Ctrl+S - Save", 200, 400, 18, sf::Color::White);
    renderer.drawText("Ctrl+L - Load", 200, 430, 18, sf::Color::White);
    renderer.drawText("ESC - Quit", 200, 460, 18, sf::Color::White);
    renderer.drawText("1/2/3 - Change Speed", 200, 490, 18, sf::Color::White);
    renderer.drawText("High Score: " + std::to_string(highScore), 200, 520, 24, sf::Color::Cyan);
    renderer.drawText("Speed: " + getSpeedString(), 200, 550, 24, sf::Color::Green);
}

void Game::renderPause() {
    renderer.drawText("PAUSED", 250, 200, 48, sf::Color::Yellow);
    renderer.drawText("Press P or SPACE to continue", 150, 260, 24, sf::Color::White);
}

void Game::renderGameOver() {
    renderer.drawText("GAME OVER", 220, 150, 48, sf::Color::Red);
    renderer.drawText("Score: " + std::to_string(score), 250, 220, 32, sf::Color::White);
    renderer.drawText("High Score: " + std::to_string(highScore), 250, 260, 24, sf::Color::Cyan);
    renderer.drawText("Press ENTER or SPACE to restart", 150, 320, 24, sf::Color::White);
    renderer.drawText("ESC to quit", 250, 360, 24, sf::Color::White);
}

void Game::renderUI() {
    renderer.drawText("Score: " + std::to_string(score), 10, 10, 20, sf::Color::White);
    renderer.drawText("High: " + std::to_string(highScore), 150, 10, 20, sf::Color::Cyan);
    renderer.drawText("Length: " + std::to_string(snake.getLength()), 300, 10, 20, sf::Color::Yellow);
    renderer.drawText("Speed: " + getSpeedString(), 500, 10, 20, sf::Color::Green);
}

void Game::setGameSpeed(GameSpeed speed) {
    currentSpeed = speed;
    timeSystem.setSpeed(speed);
    snake.setMoveInterval(getMoveIntervalForSpeed());
}

std::string Game::getSpeedString() const {
    switch (currentSpeed) {
        case GameSpeed::EASY:   return "Easy";
        case GameSpeed::NORMAL: return "Normal";
        case GameSpeed::HARD:   return "Hard";
        default:                 return "Normal";
    }
}

float Game::getMoveIntervalForSpeed() const {
    switch (currentSpeed) {
        case GameSpeed::EASY:   return 0.6f;   // 简单：0.6秒/格
        case GameSpeed::NORMAL: return 0.4f;   // 普通：0.4秒/格
        case GameSpeed::HARD:   return 0.24f;  // 困难：0.24秒/格
        default:                 return 0.4f;
    }
}
