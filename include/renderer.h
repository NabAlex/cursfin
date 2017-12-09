#ifndef CURSFIN_RENDERER_H
#define CURSFIN_RENDERER_H

#include <limits.h>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "base_drawer.h"
#include "util.h"
#include "assert.h"
#include "camera.h"

#define RENDERER_DEFAULT_NEAR   1
#define RENDERER_DEFAULT_FAR    100

class Renderer
{
public:
    Renderer(BaseDrawer *drawer);
    ~Renderer();
    
    void set_property(double far, double near);
    void update(Camera *camera);
    
    void add_model(Model *model);
    void add_line(Line line);
private:
    void projection(Point &dot);
    
    bool draw_model(Camera *camera, Model *m);
    
    void draw_triangle(Point &v1, Point &v2, Point &v3, color_t color);
    void draw_line(Point &v1, Point &v2, color_t color);
    
    Point MultiVect(const Point &A, const Point &B);
    Point barycentric(Point &A, Point &B, Point &C, Point &P);
    void ResetBuffer();
    
    BaseDrawer *drawer;
    
    double *ZBuffer;
    
    double far;
    double near;
    
    std::vector<Model*> models;
    std::vector<Line> lines;
};

#endif //CURSFIN_RENDERER_H
