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
    
    // void transform(Model *model);
    bool transform(Point &dot, Point &out, bool check);
    
    void move_to(double x, double y, double z);
    void move_on(double dx, double dy, double dz);
    
    void rotate_x(double phi);
    void rotate_y(double phi);
    void rotate_z(double phi);
    
    double get_far();
    double get_near();
    
    Point get_me();
private:
    void multi_dot(Point &V, double matrix[4][4]);
    
    void multi_matrix(double **matrix1, double **matrix2, double(**res));
    
    Point multi_vect(const Point &A, const Point &B);
    
    void reverse(double **matr);
    void camera_port(Point &target);
    void pprojection(Point &target);
    void set_view_matrix();
    void set_pproj_matrix();
    void set_screen_matrix();
    void re_calc_matrix();
    void to_screen(Point &target);
    
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
