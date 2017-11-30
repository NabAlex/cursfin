#ifndef CURSFIN_RENDERER_H
#define CURSFIN_RENDERER_H

#include <limits.h>
#include <cstdlib>

#include "util.h"

class Renderer
{
private:
    Model *model;
    
    double *ZBuffer;
    
    int Width;
    int Height;
    
    double far;
    double near;
public:
    Renderer(Model *model);
    Dot barycentric(Dot &A, Dot &B, Dot &C, Dot &P);
    Dot MultiVect(const Dot &A, const Dot &B);
    void Triangle(Dot &v1, Dot &v2, Dot &v3, Dot &Tv1, Dot &Tv2, Dot &Tv3, int rgb);
    
    void DrawAll(double far, double near);
    void ResetBuffer();
    
    ~Renderer();
};

#endif //CURSFIN_RENDERER_H
