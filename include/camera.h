#ifndef CURSFIN_CAMERA_H
#define CURSFIN_CAMERA_H

#include <vector>
#include <math.h>

#include "util.h"

class Camera
{
private:
    std::vector<Dot> *Vertex;
    std::vector<std::vector<int>> *Connections;
    
    Dot Eye;
    Dot Center;
    Dot Up;
    
    double Minv[4][4] = { { 1,0,0,0 },{ 0,1,0,0 },{ 0,0,1,0 },{ 0,0,0,1 } };
    double ViewPort[4][4] = { { 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } };
    double Screen[4][4] = { { 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } };
    
    int Width;
    int Heigth;
    double XZangle;// for hfou
    double YZangle;// for ufou
    double far;
    double near;
    
public:
    Camera(const Dot &eye, const Dot &center, const Dot &up, double XZangle, double YZangle, double far, double near, int height, int weight);
    
    void SetActive(Model* model);
    
    Camera() = default;
    ~Camera();
    
    void Transform();
    void MultiDot(Dot &V, double matrix[4][4]);
    
    void MultiMatrix(double matrix1[4][4], double matrix2[4][4], double(res[4][4]));
    
    Dot MultiVect(const Dot &A, const Dot &B);
    
    void Reverse(double matr[4][4]);
    void CameraPort(Dot &target);
    void PProjection(Dot &target);
    void SetViewMatrix();
    void SetPProjMatrix();
    void SetScreenMatrix();
    void ReCalcMatrix();
    void ToScreen(Dot &target);
    
    void MoveTo(double x, double y, double z);
    void MoveOn(double dx, double dy, double dz);
    
    void RotateX(double phi);
    void RotateY(double phi);
    void RotateZ(double phi);
    
    double getFar();
    double getNear();
};

#endif //CURSFIN_CAMERA_H
