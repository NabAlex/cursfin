#include "util.h"

Vec3i dtovec3i(Point d)
{
    return Vec3i((int) d.x, (int) d.y, (int) d.z);
}

Line::Line(Point v1, Point v2) : color(Color(COLOR_BLACK))
{
    this->v1 = v1;
    this->v2 = v2;
}

Line::Line(Point v1, Point v2, Color color) : color(color)
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
