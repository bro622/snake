#pragma once
#include <string>
#include <deque>
#include "Types.h"
#include "core/StateMachine.h"  // 使用统一的GameState定义

// 存档数据结构
struct SaveData {
    std::deque<Point> snake;
    Point foodPosition;
    Direction direction;
    int score;
    int gridWidth;
    int gridHeight;
};

class SaveSystem {
private:
    std::string saveDirectory;
    std::string saveFileName;
    
public:
    SaveSystem();
    
    // 初始化存档系统
    void initialize(const std::string& dir = "data/saves");
    
    // 保存游戏
    bool saveGame(const SaveData& data, const std::string& slotName = "save1");
    
    // 读取游戏
    bool loadGame(SaveData& data, const std::string& slotName = "save1");
    
    // 检查存档是否存在
    bool hasSave(const std::string& slotName = "save1") const;
    
    // 删除存档
    bool deleteSave(const std::string& slotName = "save1");
    
    // 获取存档文件路径
    std::string getSavePath(const std::string& slotName) const;
    
private:
    // 确保存档目录存在
    void ensureSaveDirectory();
    
    // JSON 格式序列化（文本格式）
    std::string serializeToJson(const SaveData& data) const;
    
    // JSON 格式反序列化
    bool deserializeFromJson(const std::string& json, SaveData& data) const;
};
