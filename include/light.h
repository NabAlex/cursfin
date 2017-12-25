#ifndef CURSFIN_LIGHT_H
#define CURSFIN_LIGHT_H

#include "model.h"
#include "object.h"

#define LIGHT_INTENSITY_NORMAL 5

class Light : public Object
{
private:
    Model *light_model;
    
    void set_model(double x, double y, double z);
public:
    Light(double it);
    ~Light();
    
    Light(const Light &src);
    
    void set_position(Point &p);
    void set_enable(bool enable);
    Model *get_model();
    
    bool enable;
    
    double it;
    double x, y, z;
};

#endif //CURSFIN_LIGHT_H
