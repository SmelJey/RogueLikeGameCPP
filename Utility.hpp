#pragma once
#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <set>
#include <unordered_set>
#include <queue>

class Entity;
class Player;
class Enemy;
class Projectile;

namespace util {

    struct Point {
        Point();
        Point(int xCoord, int yCoord);
        explicit Point(std::pair<int, int> coords);
        Point(const Point& src);

        Point operator+(const Point& lhs) const;
        Point& operator+=(const Point& lhs);

        Point operator-(const Point& lhs) const;
        Point& operator-=(const Point& lhs);

        Point operator*(int lhs) const;
        Point& operator*=(int lhs);

        Point operator/(int lhs) const;
        Point& operator/=(int lhs);

        bool operator==(const Point& lhs) const;

        bool operator!=(const Point& lhs) const;

        friend bool checkPoint(const std::vector<std::string>& map, const Point& p);

        Point& randomize(const std::vector<std::string>& map);
        int x, y;

        static Point Right();
        static Point Left();
        static Point Up();
        static Point Down();

        static Point URight();
        static Point ULeft();
        static Point DRight();
        static Point DLeft();
    };

    int distance(const Point& lhs, const Point& rhs);

    bool operator<(const Point& rhs, const Point& lhs);
    bool checkPoint(const std::vector<std::string>& map, const Point& p);


    struct GameInfo {
        int getNextId();
        char& operator[](const Point& pos);

        std::vector<std::string> map;

        std::vector<std::unique_ptr<Entity>> entities;
        std::deque<std::unique_ptr<Entity>> projectiles;

        int curId = 1;

        
    };
}
