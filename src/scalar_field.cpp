#include "scalar_field.h"

ScalarField::ScalarField(scalar_t scalar_type,
        double min_value,
        double max_value,
        int32_t size)
{
    const PerlinNoise perlin(time(0));
    const double fx = size / 20;
    const double fy = size / 20;
    
    int octaves = 10;
    
    field = new double*[size];
    for (int32_t i = 0; i < size; ++i)
    {
        field[i] = new double[size];
        
        if (scalar_type == SCALAR_CAVE_RANDOM)
            for (int32_t j = 0; j < size; ++j)
                field[i][j] = min_value + perlin.octaveNoise0_1(i / fx, j / fy, octaves) * max_value;
    }
}

ScalarField::~ScalarField()
{
    for (int32_t i = 0; i < size; ++i)
        delete[] field;
    
    delete field;
}
