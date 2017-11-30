#ifndef CURSFIN_CREATOR_H
#define CURSFIN_CREATOR_H

#include <random>

#include "util.h"

class Creator
{
public:
    Creator(unsigned int seed);
    ~Creator();
    
    void Generate();
    Model *GetModel();
private:
    Model *m;
};

#endif //CURSFIN_CREATOR_H
