#ifndef CURSFIN_BASE_DRAWER_H
#define CURSFIN_BASE_DRAWER_H

#include <limits.h>
#include <assert.h>

#include "util.h"

#define FRAME_NONE UINT32_MAX

struct Pixie
{
    bool act;
    Color color;
};

class BaseDrawer
{
public:
    BaseDrawer(int w, int h)
    {
        this->width = w;
        this->height = h;
        
        matrix = new Pixie*[height + 1];
        for (int i = 0; i < height + 1; ++i)
            matrix[i] = new Pixie[width + 1];
        
        refresh_frame();
    }
    
    ~BaseDrawer()
    {
        for (int i = 0; i < height; ++i)
            delete matrix[i];
        delete[] matrix;
    }
    
    void refresh_frame()
    {
        for (int i = 0; i < height + 1; ++i)
            for (int j = 0; j < width + 1; ++j)
                matrix[i][j].act = false;
    }
    
    void pixie(int x, int y, Color &color)
    {
        if (x < 0 || x > width || y < 0 || y > height)
            return;
        
        matrix[x][y].color = color;
        matrix[x][y].act = true;
    }
    
    int width;
    int height;
    
    Pixie **matrix;
};

#endif //CURSFIN_BASE_DRAWER_H
