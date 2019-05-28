#pragma once

#include <mycadlib.h>
#include <ostream>
#include <vector>
#include <sstream>
#include <cmath>
#include <iterator>
#include <iostream> // cout
#include <iomanip> // setprecision


template<class T>
class vector3 {
public:
    T x;
    T y;
    T z;

    vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    std::string str() const {
        std::ostringstream ss;
        ss << std::fixed;
        ss << std::setprecision(2);
        ss << "{" << x << "," << y << "," << z << "}";
        return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, const vector3 &rhs) {
        os << rhs.str();
        return os;
    }
};


template<class T>
class ellipse3 {
public:
    explicit ellipse3(T x, T y, T a, T b, size_t points_count = 8)
            : ellipse3(x, y, 0, a, b, 0, 360, 0, 0, 0, points_count) {}


    ///! a - major, b - minor circle
    explicit ellipse3(T x, T y, T z, T a, T b, T angle_start = 0, T angle_end = 360, T yaw = 0, T pitch = 0, T roll = 0, size_t points_count = 8)
            : x(x), y(y), z(z), a(a), b(b), angle_start(angle_start), angle_end(angle_end), yaw(yaw), pitch(pitch), roll(roll) {
        points.reserve(points_count);

        T angle_len = angle_end - angle_start;
        // T angle_step = angle_len/points_count;
        T angle_cursor = angle_start;
        for (size_t i = 0; i < points_count; i++) {
            /// x=a*cos(t), y=b*sin(t), for t from 0 to 2*pi, where a=5, b=3
            vector3 point(static_cast<T>(x + a * cos(angle_cursor * M_PI / 180)),
                          static_cast<T>(y + b * sin(angle_cursor * M_PI / 180)),
                          z);
            points.push_back(point);
            // angle_cursor += angle_step;
            angle_cursor = angle_len * (i + 1) / points_count;
        }
    }

    vector3<T> pos() {
        return vector3<T>(x, y, z);
    }

    std::string str() const {
        std::ostringstream ss;
        for (const vector3<T> &point : points)
            ss << point << ",";
        ss << "\b";
        return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, const ellipse3 &rhs) {
        os << rhs.str();
        return os;
    }

public:
    T x;
    T y;
    T z;
    T a;
    T b;
    T angle_start;
    T angle_end;
    T yaw;      // TODO: ignored
    T pitch;    // TODO: ignored
    T roll;     // TODO: ignored
    std::vector<vector3<T>> points;
};


template<class T>
class circle3 : public ellipse3<T> {
public:
    explicit circle3(T x, T y, T r, size_t points_count = 8)
            : circle3(x, y, 0, r, 0, 360, 0, 0, 0, points_count) {}

    explicit circle3(T x, T y, T z, T r, T angle_start = 0, T angle_end = 360, T yaw = 0, T pitch = 0, T roll = 0, size_t points_count = 8)
            : ellipse3<T>(x, y, z, r, r, angle_start, angle_end, yaw, pitch, roll, points_count) {}
};
