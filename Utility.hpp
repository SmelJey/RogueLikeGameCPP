#pragma once
#include <utility>
#include <vector>
#include <string>

namespace util {
    struct Point {
        Point() : x(0), y(0) {}
        Point(int xCoord, int yCoord) : x(xCoord), y(yCoord) {}
        explicit Point(std::pair<int, int> coords) : x(coords.first), y(coords.second) {}
        Point(const Point& src) : x(src.x), y(src.y) {}

        Point operator+(const Point& lhs) const {
            return Point(this->x + lhs.x, this->y + lhs.y);
        }
        Point& operator+=(const Point& lhs) {
            x += lhs.x;
            y += lhs.y;
            return *this;
        }

        Point operator-(const Point& lhs) const {
            return Point(this->x - lhs.x, this->y - lhs.y);
        }
        Point& operator-=(const Point& lhs) {
            x -= lhs.x;
            y -= lhs.y;
            return *this;
        }

        Point operator*(int lhs) const {
            return Point(this->x * lhs, this->y * lhs);
        }
        Point& operator*=(int lhs) {
            x *= lhs;
            y *= lhs;
            return *this;
        }

        bool operator==(const Point& lhs) const {
            return this->x == lhs.x && this->y == lhs.y;
        }

        bool operator!=(const Point& lhs) const {
            return !((*this) == lhs);
        }

        friend bool checkPoint(const std::vector<std::string>& map, const Point& p);

        Point& randomize(const std::vector<std::string> &map) {
            const Point d[] = { {0, 0}, {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
            size_t randIndx = rand() % 5;
            while (!checkPoint(map, *this + d[randIndx])) {
                randIndx = rand() % 5;
            }
            return (*this += d[randIndx]);
        }

        int x, y;
    };

    bool operator<(const Point& rhs, const Point& lhs) {
        if (lhs.x == rhs.x)
            return rhs.y < lhs.y;
        return rhs.x < lhs.x;
    }

    bool checkPoint(const std::vector<std::string>& map, const Point& p) {
        return p.x >= 0 && p.y >= 0 && p.y < map.size() && p.x < map[0].size();
    }
}
