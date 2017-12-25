#include "util.h"

std::map<std::string, int32_t> config;

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

void set_normal(Vec3d &vec, ...)
{
    Point *p;
    va_list l;
    
    va_start(l, vec);
    while ((p = va_arg(l, Point *)) != NULL)
        p->norm = vec.clone();
    va_end(l);
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

inline bool space(char c)
{
    return std::isspace(c);
}

inline bool notspace(char c)
{
    return !std::isspace(c);
}

std::vector<std::string> split(const std::string& s)
{
    typedef std::string::const_iterator iter;
    std::vector<std::string> ret;
    iter i = s.begin();
    while(i != s.end()){
        i = std::find_if(i, s.end(), notspace);
        iter j= std::find_if(i, s.end(), space);
        if(i != s.end()){
            ret.push_back(std::string(i, j));
            i = j;
        }
    }
    
    return ret;
}