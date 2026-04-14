#include "core/StateMachine.h"

StateMachine::StateMachine() 
    : currentState(GameState::MENU), previousState(GameState::MENU) {}

void StateMachine::setState(GameState newState) {
    if (canTransition(currentState, newState)) {
        previousState = currentState;
        currentState = newState;
    }
}

GameState StateMachine::getCurrentState() const {
    return currentState;
}

GameState StateMachine::getPreviousState() const {
    return previousState;
}

bool StateMachine::isPlaying() const {
    return currentState == GameState::PLAYING;
}

bool StateMachine::isPaused() const {
    return currentState == GameState::PAUSED;
}

bool StateMachine::isMenu() const {
    return currentState == GameState::MENU;
}

bool StateMachine::isGameOver() const {
    return currentState == GameState::GAMEOVER;
}

bool StateMachine::canTransition(GameState from, GameState to) const {
    // 定义合法的状态转换
    switch (from) {
        case GameState::MENU:
            return to == GameState::PLAYING || to == GameState::EXIT;
            
        case GameState::PLAYING:
            return to == GameState::PAUSED || to == GameState::GAMEOVER || to == GameState::EXIT;
            
        case GameState::PAUSED:
            return to == GameState::PLAYING || to == GameState::EXIT;
            
        case GameState::GAMEOVER:
            return to == GameState::PLAYING || to == GameState::MENU || to == GameState::EXIT;
            
        case GameState::EXIT:
            return false; // EXIT是终止状态
            
        default:
            return false;
    }
}

std::string StateMachine::getStateName(GameState state) const {
    switch (state) {
        case GameState::MENU: return "MENU";
        case GameState::PLAYING: return "PLAYING";
        case GameState::PAUSED: return "PAUSED";
        case GameState::GAMEOVER: return "GAMEOVER";
        case GameState::EXIT: return "EXIT";
        default: return "UNKNOWN";
    }
}
