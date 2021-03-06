#ifndef CURSFIN_CREATOR_H
#define CURSFIN_CREATOR_H

#include <random>
#include <vector>

#include "texture.h"

#include "util.h"
#include "model.h"
#include "perlin_noise.h"
#include "scalar_field.h"

class Creator
{
public:
    vector<Model*> models;
    
    Creator();
    ~Creator();
    
    void set_property(int width, int height);
    
    void generate(int32_t level_z,
            int32_t rise,
            int32_t down,
            int32_t inv);
    
    bool is_generated();
    void update_seed(unsigned int seed);
private:
    void get_point_normal(ScalarField &scalarField,
            int32_t x, int32_t y,
            double base,
            Vec3d &out);
    
    bool generated;
    
    int w, h;
    unsigned int seed;
};

#endif //CURSFIN_CREATOR_H
