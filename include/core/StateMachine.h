#pragma once
#include <string>

// 游戏状态枚举
enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAMEOVER,  // 游戏结束
    EXIT
};

class StateMachine {
private:
    GameState currentState;
    GameState previousState;
    
public:
    StateMachine();
    
    // 状态转换
    void setState(GameState newState);
    GameState getCurrentState() const;
    GameState getPreviousState() const;
    
    // 状态查询
    bool isPlaying() const;
    bool isPaused() const;
    bool isMenu() const;
    bool isGameOver() const;
    
    // 状态转换验证
    bool canTransition(GameState from, GameState to) const;
    
    // 状态名称（用于调试）
    std::string getStateName(GameState state) const;
};
