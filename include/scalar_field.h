#ifndef CURSFIN_SCALAR_FIELD_H
#define CURSFIN_SCALAR_FIELD_H

#include "util.h"
#include "perlin_noise.h"

typedef enum scalar
{
    SCALAR_NONE,
    SCALAR_CAVE_RANDOM
} scalar_t;

class ScalarField
{
public:
    ScalarField(scalar_t scalar_type,
            double min_value,
            double max_value,
            int32_t size);
    ~ScalarField();
    
    double **field;
    int32_t size;
};

#endif //CURSFIN_SCALAR_FIELD_H
