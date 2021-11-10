//
// Created by Jingtao Huang on 10/30/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_COLOR_H
#define MOSHAPE_COLOR_H

#include <Eigen/Core>

class Color : public Eigen::Vector4f
{
public:
    static Color empty() { return Color(0, 0, 0, 0); }
    static Color zero() { return Color(0, 0, 0, 0); }
    static Color black() { return Color(0, 0, 0); }
    static Color white() { return Color(1, 1, 1); }

    static Color red() { return Color(1, 0, 0); }
    static Color green() { return Color(0, 1, 0); }
    static Color blue() { return Color(0, 0, 1); }

    static Color cyan() { return Color(0, 1, 1); }
    static Color yellow() { return Color(1, 1, 0); }
    static Color purple() { return Color(1, 0, 1); }

    static Color orange() { return Color(218 / 256.0f, 130 / 256.0f, 50 / 256.0f); }

    Color(const Eigen::Vector4f& color)
        : Eigen::Vector4f(color)
    {}

    Color(const Eigen::Vector3f& rgb, float alpha = 1.0f)
        : Eigen::Vector4f(rgb.x(), rgb.y(), rgb.z(), alpha)
    {}

    Color(const float r, const float g, const float b, const float a)
        : Color(Eigen::Vector4f(r, g, b, a))
    {}

    Color(const float r, const float g, const float b)
        : Color(r, g, b, 1.0f)
    {}

    float& r() { return x(); }
    float r() const { return x(); }

    float& g() { return y(); }
    float g() const { return y(); }

    float& b() { return z(); }
    float b() const { return z(); }

    float& a() { return coeffRef(3); }
    float a() const { return coeff(3); }
};


#endif // MOSHAPE_COLOR_H
