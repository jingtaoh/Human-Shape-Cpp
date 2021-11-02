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
    Color(const Eigen::Vector4f& color)
        : Eigen::Vector4f(color)
    {}

    Color(const float r, const float g, const float b, const float a)
        : Color(Eigen::Vector4f(r, g, b, a))
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
