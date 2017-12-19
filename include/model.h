#ifndef CURSFIN_MODEL_H
#define CURSFIN_MODEL_H

#include "util.h"
#include "texture.h"

class Model
{
public:
    Point v1, v2, v3;
    
    Model() = default;
    Model(Point v1, Point v2, Point v3) : v1(v1), v2(v2), v3(v3)
    {
        this->texture = nullptr;
    }
    
    Model(Point v1, Point v2, Point v3, Texture *texture) : v1(v1), v2(v2), v3(v3)
    {
        this->texture = texture;
    }
    
    ~Model() {}
    
    Texture *getTexture()
    {
        return texture;
    }
private:
    Texture *texture;
};

#endif //CURSFIN_MODEL_H
