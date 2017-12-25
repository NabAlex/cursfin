#ifndef CURSFIN_BASE_DRAWER_H
#define CURSFIN_BASE_DRAWER_H

#include <limits.h>
#include <assert.h>

#include <gdkmm.h>

#include "util.h"

class BaseDrawer
{
public:
    BaseDrawer(int w, int h)
    {
        this->width = w;
        this->height = h;
        
        this->_size = this->width * this->height * sizeof(color_t);
        this->frame_buffer = new color_t[this->width * this->height];
    }
    
    ~BaseDrawer()
    {
        delete[] this->frame_buffer;
    }
    
    void refresh_frame()
    {
        memset(this->frame_buffer, 255, this->_size);
    }
    
    void pixie(int x, int y, color_t rgb)
    {
        if (x < 0 || x > width || y < 0 || y > height)
            return;
        
        int el = y * width + x;
        
        frame_buffer[el] = rgb;
    }
    
    size_t _size;
    
    int width;
    int height;
    
    color_t *frame_buffer;
};

#endif //CURSFIN_BASE_DRAWER_H
