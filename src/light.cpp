#include "light.h"

Light::Light(double it) : it(it), x(0), y(0), z(0), enable(true)
{
    Point v1(x, y, z, COLOR_RED), v2(x + 1, y + 1, z + 1, COLOR_YELLOW), v3(x + 1, y, z + 1, COLOR_YELLOW);
    light_model = new Model(v1, v2, v3);
}

Light::~Light()
{
    delete light_model;
}

Light::Light(const Light &src)
{
    this->it = src.it;
    
    this->x = src.x;
    this->y = src.y;
    this->z = src.z;
}

Model *Light::get_model()
{
    return light_model;
}

void Light::set_position(Point &p)
{
    this->x = p.x;
    this->y = p.y;
    this->z = p.z;
}

void Light::set_enable(bool enable)
{
    this->enable = enable;
}
