#include "SaveSystem.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

SaveSystem::SaveSystem() {}

void SaveSystem::initialize(const std::string& dir) {
    saveDirectory = dir;
    ensureSaveDirectory();
}

void SaveSystem::ensureSaveDirectory() {
    // 确保存档目录存在
    if (!fs::exists(saveDirectory)) {
        fs::create_directories(saveDirectory);
    }
}

bool SaveSystem::saveGame(const SaveData& data, const std::string& slotName) {
    ensureSaveDirectory();
    
    std::string filePath = getSavePath(slotName);
    std::ofstream file(filePath);
    
    if (!file.is_open()) {
        return false;
    }
    
    // 使用 JSON 格式序列化（文本格式，易读）
    std::string json = serializeToJson(data);
    file << json;
    
    file.close();
    return true;
}

bool SaveSystem::loadGame(SaveData& data, const std::string& slotName) {
    std::string filePath = getSavePath(slotName);
    
    if (!fs::exists(filePath)) {
        return false;
    }
    
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string json = buffer.str();
    file.close();
    
    return deserializeFromJson(json, data);
}

bool SaveSystem::hasSave(const std::string& slotName) const {
    std::string filePath = getSavePath(slotName);
    return fs::exists(filePath);
}

bool SaveSystem::deleteSave(const std::string& slotName) {
    std::string filePath = getSavePath(slotName);
    
    if (fs::exists(filePath)) {
        fs::remove(filePath);
        return true;
    }
    return false;
}

std::string SaveSystem::getSavePath(const std::string& slotName) const {
    return saveDirectory + "/" + slotName + ".json";
}

std::string SaveSystem::serializeToJson(const SaveData& data) const {
    std::stringstream ss;
    
    // 手动构建简单的 JSON 格式
    ss << "{\n";
    ss << "  \"score\": " << data.score << ",\n";
    ss << "  \"direction\": " << static_cast<int>(data.direction) << ",\n";
    ss << "  \"gridWidth\": " << data.gridWidth << ",\n";
    ss << "  \"gridHeight\": " << data.gridHeight << ",\n";
    
    // 蛇身数组
    ss << "  \"snake\": [";
    for (size_t i = 0; i < data.snake.size(); ++i) {
        ss << "[" << data.snake[i].x << "," << data.snake[i].y << "]";
        if (i < data.snake.size() - 1) {
            ss << ",";
        }
    }
    ss << "],\n";
    
    // 食物位置
    ss << "  \"food\": [" << data.foodPosition.x << "," << data.foodPosition.y << "]\n";
    
    ss << "}";
    
    return ss.str();
}

bool SaveSystem::deserializeFromJson(const std::string& json, SaveData& data) const {
    // 简单的 JSON 解析（针对我们自己的格式）
    // 实际项目中建议使用 nlohmann/json 等库
    
    try {
        // 提取 score
        size_t pos = json.find("\"score\":");
        if (pos != std::string::npos) {
            pos += 8;
            size_t end = json.find(',', pos);
            data.score = std::stoi(json.substr(pos, end - pos));
        }
        
        // 提取 direction
        pos = json.find("\"direction\":");
        if (pos != std::string::npos) {
            pos += 12;
            size_t end = json.find(',', pos);
            data.direction = static_cast<Direction>(std::stoi(json.substr(pos, end - pos)));
        }
        
        // 提取 grid 尺寸
        pos = json.find("\"gridWidth\":");
        if (pos != std::string::npos) {
            pos += 12;
            size_t end = json.find(',', pos);
            data.gridWidth = std::stoi(json.substr(pos, end - pos));
        }
        
        pos = json.find("\"gridHeight\":");
        if (pos != std::string::npos) {
            pos += 13;
            size_t end = json.find(',', pos);
            data.gridHeight = std::stoi(json.substr(pos, end - pos));
        }
        
        // 提取蛇身
        data.snake.clear();
        pos = json.find("\"snake\":");
        if (pos != std::string::npos) {
            size_t start = json.find('[', pos) + 1;
            size_t end = json.find(']', start);
            std::string snakeStr = json.substr(start, end - start);
            
            // 解析每个点
            size_t currPos = 0;
            while ((currPos = snakeStr.find('[')) != std::string::npos) {
                size_t comma = snakeStr.find(',', currPos);
                size_t closeBracket = snakeStr.find(']', currPos);
                
                if (comma != std::string::npos && closeBracket != std::string::npos) {
                    int x = std::stoi(snakeStr.substr(currPos + 1, comma - currPos - 1));
                    int y = std::stoi(snakeStr.substr(comma + 1, closeBracket - comma - 1));
                    data.snake.push_back(Point(x, y));
                    
                    snakeStr = snakeStr.substr(closeBracket + 1);
                } else {
                    break;
                }
            }
        }
        
        // 提取食物位置
        pos = json.find("\"food\":");
        if (pos != std::string::npos) {
            size_t start = json.find('[', pos) + 1;
            size_t comma = json.find(',', start);
            size_t end = json.find(']', start);
            
            if (comma != std::string::npos && end != std::string::npos) {
                int x = std::stoi(json.substr(start, comma - start));
                int y = std::stoi(json.substr(comma + 1, end - comma - 1));
                data.foodPosition = Point(x, y);
            }
        }
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}
