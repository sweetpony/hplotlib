#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

namespace hpl
{
struct Geometry
{
    Geometry() : leftOffset(0), topOffset(0), width(1.0), height(1.0) {}
    Geometry(float leftOffset, float rightOffset, float width, float height) : leftOffset(leftOffset), topOffset(rightOffset), width(width), height(height) {}

    void set(float leftOffset, float rightOffset, float width, float height) {
        this->leftOffset = leftOffset;
        this->topOffset = rightOffset;
        this->width = width;
        this->height = height;
    }

    float leftOffset, topOffset, width, height;
};
}

#endif // GEOMETRY_HPP
