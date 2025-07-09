#pragma once

// �ݲ�ʹ�ã���ʱ����д��������ѧ��
namespace Cube {
    struct Vec2 {
        float x, y;

        Vec2 operator+(const Vec2& other);
        Vec2 operator-(const Vec2& other);
        float operator*(const Vec2& other);
        float cross(const Vec2& other);
    };

    struct Vec3 {
        float x, y, z;

        Vec3 operator+(const Vec3& other);
        Vec3 operator-(const Vec3& other);
        float operator*(const Vec3& other);
        Vec3 cross(const Vec3& other);
    };

    struct Vec4 {
        float x, y, z, w;

        Vec4 operator+(const Vec4& other);
        Vec4 operator-(const Vec4& other);
        float operator*(const Vec4& other);
    };

    // �����Ⱦ���
    struct Mat2 {
        float data[4];

        Mat2 operator+(const Mat2& other);
        Mat2 operator-(const Mat2& other);
        Mat2 operator*(const Mat2& other);
        Vec2 operator*(const Vec2& other);
        Mat2 inverse() const;
    };

    struct Mat3 {
        float data[9];

        Mat3 operator+(const Mat3& other);
        Mat3 operator-(const Mat3& other);
        Mat3 operator*(const Mat3& other);
        Vec3 operator*(const Vec3& other);
        Mat3 inverse() const;
    };

    struct Mat4 {
        float data[16];

        Mat4 operator+(const Mat4& other);
        Mat4 operator-(const Mat4& other);
        Mat4 operator*(const Mat4& other);
        Vec4 operator*(const Vec4& other);
        Mat4 inverse() const;
    };
}  // namespace Cube
