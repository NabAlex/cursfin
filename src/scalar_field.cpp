#include "scalar_field.h"

ScalarField::ScalarField(scalar_t scalar_type,
        unsigned int seed,
        double min_value,
        double max_value,
        int32_t size) : size(size)
{
    const PerlinNoise perlin(seed);
    const double fx = size / 20;
    const double fy = size / 20;
    
    int octaves = config["octaves"];
    
    field = new double*[size];
    for (int32_t i = 0; i < size; ++i)
    {
        field[i] = new double[size];
        
        if (scalar_type == SCALAR_CAVE_RANDOM)
            for (int32_t j = 0; j < size; ++j)
                field[i][j] = min_value + perlin.octaveNoiseCave(i / fx, j / fy, octaves) * max_value;
    }
}

ScalarField::~ScalarField()
{
    for (int32_t i = 0; i < size; ++i)
        delete field[i];
    
    delete[] field;
}
