#pragma once
struct Vec2 {
    float x, y;

    const Vec2& operator+(const Vec2& other);
    const Vec2& operator-(const Vec2& other);
    const Vec2& operator*(const Vec2& other);
    float dot(const Vec2& x);
};

struct Vec3 {
    float x, y, z;

    const Vec3& operator+(const Vec3& other);
    const Vec3& operator-(const Vec3& other);
    const Vec3& operator*(const Vec3& other);
    float dot(const Vec3& x);
    const Vec3& cross(const Vec3& x);
};