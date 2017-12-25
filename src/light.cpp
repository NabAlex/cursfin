#include "light.h"

Light::Light(double it) : it(it), x(0), y(0), z(0), light_model(nullptr), enable(true)
{}

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
    
    set_model(x, y, z);
}

void Light::set_model(double x, double y, double z)
{
    Point v1(x, y, z, COLOR_RED), v2(x + 1, y + 1, z + 1, COLOR_YELLOW), v3(x + 1, y, z + 1, COLOR_YELLOW);
    
    if (light_model == nullptr)
        light_model = new Model(v1, v2, v3);
    else
    {
        light_model->v1 = v1;
        light_model->v2 = v2;
        light_model->v3 = v3;
    }
}

void Light::set_enable(bool enable)
{
    this->enable = enable;
}
