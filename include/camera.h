#ifndef CURSFIN_CAMERA_H
#define CURSFIN_CAMERA_H

#include <vector>
#include <math.h>

#include "util.h"

class Camera
{
public:
    Camera(const Point &eye, const Point &center, const Point &up, double XZangle, double YZangle, double far, double near, int w, int h);
    
    Camera() = default;
    ~Camera();
    
    // void Transform(Model *model);
    bool Transform(Point &dot, Point &out);
    
    void MoveTo(double x, double y, double z);
    void MoveOn(double dx, double dy, double dz);
    
    void RotateX(double phi);
    void RotateY(double phi);
    void RotateZ(double phi);
    
    double getFar();
    double getNear();
private:
    void MultiDot(Point &V, double matrix[4][4]);
    
    void MultiMatrix(double matrix1[4][4], double matrix2[4][4], double(res[4][4]));
    
    Point MultiVect(const Point &A, const Point &B);
    
    void Reverse(double matr[4][4]);
    void CameraPort(Point &target);
    void PProjection(Point &target);
    void SetViewMatrix();
    void SetPProjMatrix();
    void SetScreenMatrix();
    void ReCalcMatrix();
    void ToScreen(Point &target);
    
    Point Eye;
    Point Center;
    Point Up;
    
    double Minv[4][4] = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
    
    double ViewPort[4][4] = { { 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } };
    double Screen[4][4] = { { 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } };
    
    int width;
    int heigth;
    double XZangle;// for hfou
    double YZangle;// for ufou
    double far;
    double near;
};

#endif //CURSFIN_CAMERA_H
