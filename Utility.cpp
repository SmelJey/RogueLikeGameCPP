#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <set>
#include "Utility.hpp"
#include "Enemy.hpp"

class Entity;
class Player;
class Projectile;

namespace util {
    Point::Point() : x(0), y(0) {}
    Point::Point(int xCoord, int yCoord) : x(xCoord), y(yCoord) {}
    Point::Point(std::pair<int, int> coords) : x(coords.first), y(coords.second) {}
    Point::Point(const Point& src) : x(src.x), y(src.y) {}

    Point Point::operator+(const Point& lhs) const {
        return Point(this->x + lhs.x, this->y + lhs.y);
    }
    Point& Point::operator+=(const Point& lhs) {
        x += lhs.x;
        y += lhs.y;
        return *this;
    }

    Point Point::operator-(const Point& lhs) const {
        return Point(this->x - lhs.x, this->y - lhs.y);
    }
    Point& Point::operator-=(const Point& lhs) {
        x -= lhs.x;
        y -= lhs.y;
        return *this;
    }

    Point Point::operator*(int lhs) const {
        return Point(this->x * lhs, this->y * lhs);
    }
    Point& Point::operator*=(int lhs) {
        x *= lhs;
        y *= lhs;
        return *this;
    }

    Point Point::operator/(int lhs) const {
        return Point(this->x / lhs, this->y / lhs);
    }
    Point& Point::operator/=(int lhs) {
        x /= lhs;
        y /= lhs;
        return *this;
    }

    bool Point::operator==(const Point& lhs) const {
        return this->x == lhs.x && this->y == lhs.y;
    }

    bool Point::operator!=(const Point& lhs) const {
        return !((*this) == lhs);
    }

    Point& Point::randomize(const std::vector<std::string>& map) {
        const Point d[] = { {0, 0}, {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
        size_t randIndx = rand() % 5;
        while (!checkPoint(map, *this + d[randIndx])) {
            randIndx = rand() % 5;
        }
        return (*this += d[randIndx]);
    }

    Point Point::Right() { return Point(1, 0); }
    Point Point::Left() { return Point(-1, 0); }
    Point Point::Up() { return Point(0, -1); }
    Point Point::Down() { return Point(0, 1); }

    Point Point::URight() { return Point(1, -1); }
    Point Point::ULeft() { return Point(-1, -1); }
    Point Point::DRight() { return Point(1, 1); }
    Point Point::DLeft() { return Point(-1, 1); }

    bool operator<(const Point& rhs, const Point& lhs) {
        if (lhs.x == rhs.x)
            return rhs.y < lhs.y;
        return rhs.x < lhs.x;
    }

    int distance(const Point& lhs, const Point& rhs) {
        return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y);
    }

    bool checkPoint(const std::vector<std::string>& map, const Point& p) {
        return p.x >= 0 && p.y >= 0 && p.y < map.size() && p.x < map[0].size();
    }

    int GameInfo::getNextId() {
        this->curId++;
        return this->curId - 1;
    }

    char& GameInfo::operator[](const Point& pos) {
        return map[pos.y][pos.x];
    }
}
