#include "util.h"

Vec3i dtovec3i(Point d)
{
    return Vec3i((int) d.x, (int) d.y, (int) d.z);
}

Line::Line(Point v1, Point v2) : color(COLOR_BLACK)
{
    this->v1 = v1;
    this->v2 = v2;
}

Line::Line(Point v1, Point v2, color_t color) : color(color)
{
    this->v1 = v1;
    this->v2 = v2;
}

Line::~Line()
{}

Line::Line(const Line &l)
{
    this->v1 = l.v1;
    this->v2 = l.v2;
    
    this->color = l.color;
}

void multi_vect(const Vec3d &A, const Vec3d &B, Vec3d &out) {
    out.x = A.y * B.z - A.z * B.y;
    out.y = A.z * B.x - A.x * B.z;
    out.z = A.x * B.y - A.y * B.x;
}

void get_normal(Point &p1, Point &p2, Point &p3, Vec3d &out)
{
    Vec3d v1(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
    Vec3d v2(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);
    multi_vect(v1, v2, out);
    out.normalize();
}

double clamp(double a, double left, double right)
{
    if (a < left)
        return left;
    
    if (a > right)
        return right;
    
    return a;
}