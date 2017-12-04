#include <iostream>
#include "camera.h"

Camera::~Camera()
{}

Camera::Camera(const Point &eye, const Point &center, const Point &up, double XZangle, double YZangle, double far, double near, int w, int h)
{
    Eye = eye;
    Center = center;
    Up = up;
    
    width = w;
    heigth = h;
    this->XZangle = XZangle;
    this->YZangle = YZangle;
    this->far = far;
    this->near = near;
    
    ReCalcMatrix();
}


bool Camera::Transform(Point &dot, Point &out)
{
    out = dot;
    Point &result = out;
    
    CameraPort(result);
    PProjection(result);
    
    if (result.w < 0)
        std::cout << "w < 0" << std::endl;
    
    result.w = fabs(result.w);
    
    if (!IN_INTERVAL(result.x, -result.w, result.w))
    {
        // x = x0 + mt
        // ... nt
        // ... pt
        // t = - (A * x1 + B * y1 + C * z1 + D) / (A * m + B * n + C * p)
        
        // or binary
        
        return false;
    }
    if (!IN_INTERVAL(result.y, -result.w, result.w))
        return false;
    
    if (!IN_INTERVAL(result.z, -result.w, result.w))
        return false;
    
    result.x /= result.w;
    result.y /= result.w;
    result.z /= result.w;
    result.w = 1;
    
    ToScreen(result);
    
    return true;
}

void Camera::Reverse(double matr[4][4]) {
    double det = matr[0][0] * matr[1][1] * matr[2][2] - matr[0][0] * matr[1][2] * matr[2][1]
        - matr[0][1] * matr[1][0] * matr[2][2] + matr[0][1] * matr[1][2] * matr[2][0]
        + matr[0][2] * matr[1][0] * matr[2][1] - matr[0][2] * matr[1][1] * matr[2][0];
    
    
    double dopoln[3][3] = { { matr[1][1] * matr[2][2] - matr[1][2] * matr[2][1],-matr[1][0] * matr[2][2] + matr[2][0] * matr[1][2],matr[1][0] * matr[2][1] - matr[1][1] * matr[2][0] },
        { -matr[0][1] * matr[2][2] + matr[0][2] * matr[2][1],matr[0][0] * matr[2][2] - matr[2][0] * matr[0][2],-matr[0][0] * matr[2][1] + matr[0][1] * matr[2][0] },
        { matr[0][1] * matr[1][2] - matr[1][1] * matr[0][2],-matr[0][0] * matr[1][2] + matr[1][0] * matr[0][2],matr[0][0] * matr[1][1] - matr[1][0] * matr[0][1] }
    };
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matr[j][i] = (dopoln[i][j] / det);
        }
    }
}


void Camera::MultiMatrix(double matrix1[4][4], double matrix2[4][4], double(res[4][4])) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int r = 0; r < 4; r++)
                res[i][j] += matrix1[i][r] * matrix2[r][j];
}


void Camera::MultiDot(Point &V, double matrix[4][4]) {
    Point new_dot = V;
    new_dot.x = matrix[0][0] * V.x + matrix[1][0] * V.y + matrix[2][0] * V.z + matrix[3][0] * V.w;
    new_dot.y = matrix[0][1] * V.x + matrix[1][1] * V.y + matrix[2][1] * V.z + matrix[3][1] * V.w;
    new_dot.z = matrix[0][2] * V.x + matrix[1][2] * V.y + matrix[2][2] * V.z + matrix[3][2] * V.w;
    new_dot.w = matrix[0][3] * V.x + matrix[1][3] * V.y + matrix[2][3] * V.z + matrix[3][3] * V.w;
    V = new_dot;
}


void Camera::SetPProjMatrix() {
    double right = tan(XZangle / 2);
    double left = -right;
    
    double top = tan(YZangle / 2);
    double bottom = -top;
    
    ViewPort[0][0] = 2. / (right - left);
    ViewPort[1][1] = 2. / (top - bottom);
    ViewPort[2][0] = (left + right) / (left - right);
    ViewPort[2][1] = (top + bottom) / (bottom - top);
    ViewPort[2][2] = (far + near) / (far - near);
    ViewPort[2][3] = 1;
    ViewPort[3][2] = -2. * near * far / (far - near);
}


void Camera::CameraPort(Point &target) {
    
    target.x -= Eye.x;
    target.y -= Eye.y;
    target.z -= Eye.z;
    
    MultiDot(target, Minv);
}


Point Camera::MultiVect(const Point &A, const Point &B) {
    Point Temp;
    Temp.x = A.y*B.z - A.z*B.y;
    Temp.y = A.z*B.x - A.x*B.z;
    Temp.z = A.x*B.y - A.y*B.x;
    return Temp;
}

void Camera::SetViewMatrix() {
    Point VectZ = (Center - Eye);
    VectZ.normalize();
    
    Point VectX = MultiVect(Up, VectZ);
    VectX.normalize();
    
    Point VectY = MultiVect(VectZ, VectX);
    VectY.normalize();
    
    Minv[0][0] = VectX.x;
    Minv[1][0] = VectX.y;
    Minv[2][0] = VectX.z;
    
    Minv[0][1] = VectY.x;
    Minv[1][1] = VectY.y;
    Minv[2][1] = VectY.z;
    
    Minv[0][2] = VectZ.x;
    Minv[1][2] = VectZ.y;
    Minv[2][2] = VectZ.z;
}


void Camera::ReCalcMatrix() {
    SetScreenMatrix();
    SetPProjMatrix();
    SetViewMatrix();
}

void Camera::PProjection(Point &target) {
    MultiDot(target, ViewPort);
}

void  Camera::SetScreenMatrix() {
    Screen[0][0] = double(width - 1) / 2;
    Screen[1][1] = double(heigth - 1) / 2;
    Screen[2][2] = 1;
    Screen[3][0] = double(width - 1) / 2;
    Screen[3][1] = double(heigth - 1) / 2;
    Screen[3][3] = 1;
}


void  Camera::ToScreen(Point &target) {
    MultiDot(target, Screen);
}

void Camera::MoveTo(double x, double y, double z) {
    Center = Point(x + Center.x - Eye.x, y + Center.y - Eye.y, z + Center.z - Eye.z);
    Eye = Point(x, y, z);
    SetViewMatrix();
}

void Camera::MoveOn(double dx, double dy, double dz) {
    Point VectZ = (Center - Eye);
    VectZ.normalize();
    
    Point VectX = MultiVect(Up, VectZ);
    VectX.normalize();
    
    Point VectY = MultiVect(VectZ, VectX);
    VectY.normalize();
    
    VectY = VectY * dy;
    VectX = VectX * dx;
    VectZ = VectZ * dz;
    
    Point Vect = VectX + VectY + VectZ;
    
    Eye = Eye + Vect;
    Center = Center + Vect;
    
    ReCalcMatrix();
}

void Camera::RotateX(double phi) {
    phi = phi / 180 * M_PI;
    double matrix[4][4] = {
        { 1,0,0,0 },
        { 0,cos(phi),sin(phi),0 },
        { 0,-sin(phi),cos(phi),0 },
        { 0,0,0,1 }
    };
    
    Center.x -= Eye.x;
    Center.y -= Eye.y;
    Center.z -= Eye.z;
    MultiDot(Center, matrix);
    Center.x += Eye.x;
    Center.y += Eye.y;
    Center.z += Eye.z;
    
    MultiDot(Up, matrix);
    
    SetViewMatrix();
}

void Camera::RotateY(double phi) {
    phi = phi / 180 * M_PI;
    double matrix[4][4] = {
        { cos(phi),0,-sin(phi),0 },
        { 0,1,0,0 },
        { sin(phi),0,cos(phi),0 },
        { 0,0,0,1 }
    };
    
    Center.x -= Eye.x;
    Center.y -= Eye.y;
    Center.z -= Eye.z;
    MultiDot(Center, matrix);
    Center.x += Eye.x;
    Center.y += Eye.y;
    Center.z += Eye.z;
    
    MultiDot(Up, matrix);
    
    SetViewMatrix();
}

void Camera::RotateZ(double phi) {
    phi = phi / 180 * M_PI;
    double matrix[4][4] = {
        { cos(phi),sin(phi),0,0 },
        { -sin(phi),cos(phi),0,0 },
        { 0,0,1,0 },
        { 0,0,0,1 }
        
    };
    
    Center.x -= Eye.x;
    Center.y -= Eye.y;
    Center.z -= Eye.z;
    MultiDot(Center, matrix);
    Center.x += Eye.x;
    Center.y += Eye.y;
    Center.z += Eye.z;
    
    MultiDot(Up, matrix);
    SetViewMatrix();
}

double Camera::getFar() {
    return far;
}

double Camera::getNear() {
    return near;
}