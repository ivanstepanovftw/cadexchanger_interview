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
        os << "{" << rhs.x << "," << rhs.y << "," << rhs.z << "}";
        return os;
    }
};


template<class T>
class curve3 {
public:
    curve3(T x, T y, T z)
            : position(x, y, z) {}

    virtual ~curve3() = default;

    virtual vector3<T> calculate(double angle) const = 0;

    virtual vector3<T> derivative(double angle) const = 0;

public:
    vector3<T> position;
};


template<class T>
class ellipse3 : public curve3<T> {
public:
    ellipse3(T x, T y, T a, T b)
            : ellipse3(x, y, static_cast<T>(0), a, b) {}

    ellipse3(T x, T y, T z, T a, T b)
            : curve3<T>(x, y, z), a(a), b(b) {}

    vector3<T> calculate(double angle) const override {
        /// x=a*cos(t), y=b*sin(t), for t from 0 to 2*pi, where a=5, b=3
        return vector3<T>(static_cast<T>(this->position.x + a * cos(angle)),
                          static_cast<T>(this->position.y + b * sin(angle)),
                          this->position.z);
    }

    vector3<T> derivative(double angle) const override {
        return vector3<T>(static_cast<T>(a * -sin(angle)),
                          static_cast<T>(b * cos(angle)),
                          0);
    }

public:
    T a;
    T b;
};


template<class T>
class circle3 : public ellipse3<T> {
public:
    explicit circle3(T x, T y, T r)
            : circle3(x, y, 0, r) {}

    explicit circle3(T x, T y, T z, T r)
            : ellipse3<T>(x, y, z, r, r) {}
};


template<class T>
// class helix3 : public ellipse<T, __Radians> <-- this is mistake, because first and last points in circle/ellipse are closed (замкнуты)
class helix3 : public curve3<T> {
public:
    explicit helix3(T x, T y, T z, T r, T step)
            : helix3<T>(x, y, z, r, r, step, 0) {}

    explicit helix3(T x, T y, T z, T a, T b, T step, double angle_start)
            : curve3<T>(x, y, z), a(a), b(b), step(step), angle_start(angle_start) {}

    vector3<T> calculate(double angle) const override {
        /// x=a*cos(t), y=b*sin(t), for t from 0 to 2*pi, where a=5, b=3
        return vector3<T>(
                static_cast<T>(this->position.x + a * cos(angle)),
                static_cast<T>(this->position.y + b * sin(angle)),
                // note changed here in difference with ellipse3::calculate()
                this->position.z + (angle_start + angle)/(M_PIl*2)*step
        );
    }

    vector3<T> derivative(double angle) const override {
        return vector3<T>(
                static_cast<T>(a * -sin(angle)),
                static_cast<T>(b * cos(angle)),
                // https://www.wolframalpha.com/input/?i=d%2Fdx+z+%2B+(f+%2B+x)%2F360*s
                step/(M_PIl*2)
        );
    }

public:
    T a;
    T b;
    T step;
    double angle_start;
};
