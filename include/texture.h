#ifndef CURSFIN_TEXTURE_H
#define CURSFIN_TEXTURE_H

#include <iostream>

#include "util.h"

#include "perlin_noise.h"

class Texture
{
public:
    Texture(size_t generate_size);
    // TODO
    // ~Texture();
    
    virtual color_t get_safe(double u, double v);
    
    color_t **texture;
    size_t heigth, width;
    
protected:
    void init(size_t w, size_t h);
    void fill(int32_t startx, int32_t starty, int32_t w, int32_t h, color_t color);
};

class StoneTexture : public Texture
{
public:
    StoneTexture(size_t generate_size, size_t fill_w);
    
    // TODO
    // ~
    
    color_t get_safe(double u, double v) override;

private:
    size_t fill_count_width;
    size_t fill_real_size;
    
    color_t **fill_map;
};

#endif //CURSFIN_TEXTURE_H
