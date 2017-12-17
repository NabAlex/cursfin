#ifndef CURSFIN_LIGHT_H
#define CURSFIN_LIGHT_H

class Light
{
public:
    Light(double x, double y, double z);
    ~Light();
    
    Light(const Light &src);
    
    double x, y, z;
};

#endif //CURSFIN_LIGHT_H
