#pragma once

// 方向枚举
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

// 点结构 - 用于表示坐标
struct Point {
    int x;
    int y;

    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    Point& operator+(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
};
