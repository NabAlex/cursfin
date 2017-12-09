#ifndef CURSFIN_CREATOR_H
#define CURSFIN_CREATOR_H

#include <random>

#include "util.h"

class Creator
{
public:
    Creator(size_t max_polygons, unsigned int seed);
    ~Creator();
    
    void setProperty(int width, int height);
    
    void Generate();
    Model **GetModels();
    size_t SizeModels();
    
    bool isGenerated();
private:
    bool generated;
    Model *m;
    
    Model **models;
    size_t models_size;
    
    int w, h;
};

#endif //CURSFIN_CREATOR_H
