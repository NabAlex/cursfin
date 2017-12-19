#ifndef CURSFIN_LIGHT_H
#define CURSFIN_LIGHT_H

#include "model.h"

#define LIGHT_INTENSITY_NORMAL 5

class Light
{
private:
    Model *light_model;
public:
    Light(double it, double x, double y, double z);
    ~Light();
    
    Light(const Light &src);
    
    Model *get_model();
    
    double it;
    
    double x, y, z;
};

#endif //CURSFIN_LIGHT_H
