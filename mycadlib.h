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


//! for visuals
template<class T>
class mappable3 {
public:
    virtual void remap(size_t points_count) {
        this->points.clear();
        this->points.reserve(points_count);
    }

    std::string str() const {
        std::ostringstream ss;
        for (const vector3<T> &point : points)
            ss << point << ",";
        ss << "\b";
        return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, const mappable3 &rhs) {
        os << rhs.str();
        return os;
    }

public:
    std::vector<vector3<T>> points;
};

// TODO: curve3 should not inherit mappable3
template<class T>
class curve3 : public virtual mappable3<T> {
public:
    curve3(T x, T y, T z, T angle_start, T angle_end, size_t points_count)
            : position(x, y, z), angle_start(angle_start), angle_end(angle_end) {}

    virtual vector3<T> calculate(T angle) const = 0;

    virtual vector3<T> derivative(T angle) const = 0;

public:
    vector3<T> position;
    T angle_start;
    T angle_end;
};


template<class T>
class ellipse3 : public curve3<T> {
public:
    ellipse3(T x, T y, T a, T b, size_t points_count)
            : ellipse3(x, y, 0, a, b, 0, 360, points_count) {}

    ellipse3(T x, T y, T z, T a, T b, T angle_start, T angle_end, size_t points_count)
            : curve3<T>(x, y, z, angle_start, angle_end, points_count), a(a), b(b) {
        remap(points_count);
    }

    void remap(size_t points_count) override {
        mappable3<T>::remap(points_count);

        T angle_len = this->angle_end - this->angle_start;
        T angle_cursor = this->angle_start;
        for (size_t i = 0; i < points_count; i++) {
            vector3 point = calculate(angle_cursor);
            this->points.push_back(point);
            angle_cursor = angle_len * (i + 1) / points_count;
        }
    }

    vector3<T> calculate(T angle) const override {
        /// x=a*cos(t), y=b*sin(t), for t from 0 to 2*pi, where a=5, b=3
        return vector3<T>(static_cast<T>(this->position.x + this->a * cos(angle * M_PI / 180)),
                          static_cast<T>(this->position.y + this->b * sin(angle * M_PI / 180)),
                          this->position.z);
    }

    vector3<T> derivative(T angle) const override {
        return vector3<T>(static_cast<T>(this->a * -sin(angle * M_PI / 180)),
                          static_cast<T>(this->b * cos(angle * M_PI / 180)),
                          0);
    }

public:
    T a;
    T b;
};


template<class T>
class circle3 : public ellipse3<T> {
public:
    explicit circle3(T x, T y, T r, size_t points_count)
            : circle3(x, y, 0, r, 0, 360, 0, 0, 0, points_count) {}

    explicit circle3(T x, T y, T z, T r, T angle_start, T angle_end, size_t points_count)
            : ellipse3<T>(x, y, z, r, r, angle_start, angle_end, points_count) {}
};


template<class T>
// class helix3 : public ellipse<T> <-- this is mistake, because first and last points in circle/ellipse are closed (замкнуты)
class helix3 : public curve3<T> {
public:
    explicit helix3(T x, T y, T z, T r, T step, size_t points_count)
            : helix3<T>(x, y, z, r, r, step, 0, 360, points_count) {}

    explicit helix3(T x, T y, T z, T a, T b, T step, T angle_start, T angle_end, size_t points_count)
            : curve3<T>(x, y, z, angle_start, angle_end, points_count), a(a), b(b), step(step) {
        remap(points_count);
    }

    void remap(size_t points_count) override {
        curve3<T>::remap(points_count);

        T angle_len = this->angle_end - this->angle_start;
        T angle_cursor = this->angle_start;
        for (size_t i = 0; i < points_count; i++) {
            vector3 point = calculate(angle_cursor);
            this->points.push_back(point);
            angle_cursor = angle_len * (i + 1) / (points_count - 1); // note changed here in difference with ellipse3::remap()
        }
    }

    vector3<T> calculate(T angle) const override {
        /// x=a*cos(t), y=b*sin(t), for t from 0 to 2*pi, where a=5, b=3
        return vector3<T>(
                static_cast<T>(this->position.x + this->a * cos(angle * M_PI / 180)),
                static_cast<T>(this->position.y + this->b * sin(angle * M_PI / 180)),
                // note changed here in difference with ellipse3::calculate()
                this->position.z + (this->angle_start + angle)/360*step
                // for the height:
                // this->position.z + (this->angle_start + angle)/(this->angle_end - this->angle_start)*height
        );
    }

    vector3<T> derivative(T angle) const override {
        return vector3<T>(
                static_cast<T>(this->a * -sin(angle * M_PI / 180)),
                static_cast<T>(this->b * cos(angle * M_PI / 180)),
                // https://www.wolframalpha.com/input/?i=d%2Fdx+z+%2B+(f+%2B+x)%2F360*s
                step/360
        );
    }

protected:
    T a;
    T b;
    T step;
};
