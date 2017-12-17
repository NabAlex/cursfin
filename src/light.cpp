#include "light.h"

Light::Light(double x, double y, double z) : x(x), y(y), z(z)
{}

Light::~Light()
{}

Light::Light(const Light &src)
{
    this->x = src.x;
    this->y = src.y;
    this->z = src.z;
}
