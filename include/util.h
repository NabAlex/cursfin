#ifndef CURSFIN_UTIL_H
#define CURSFIN_UTIL_H

#include <vector>
#include <math.h>
#include <stdint-gcc.h>
#include <random>
#include <iostream>
#include <stdarg.h>
#include <algorithm>
#include <map>

using namespace std;

extern std::map<std::string, int32_t> config;

#define SIZE_OF_ARRAY(ar) (sizeof((ar)) / sizeof(*(ar)))

/* work with COLOR */
typedef uint32_t color_t;

inline color_t color_pack(int32_t r, int32_t g, int32_t b)
{
    return (255 << 24) | (b << 16) | (g << 8) | (r);
}

#define ONLY_PRESS(event, button) ((event)->type == GDK_KEY_PRESS && (event)->keyval == (button) && !(event)->state)

#define COLOR_UNPACK_R(c) ((c) & 0xFF)
#define COLOR_UNPACK_G(c) (((c) >> 8) & 0xFF)
#define COLOR_UNPACK_B(c) (((c) >> 16) & 0xFF)

#define COLOR_MULTI(c, k) color_pack(((c) & 0xFF) * (k), (((c) >> 8) & 0xFF) * (k), (((c) >> 16) & 0xFF) * (k))
#define COLOR_MULTI_SAFE(min, c, k) color_pack(min(240, (int32_t) (((c) & 0xFF) * (k))), min(240, (int32_t) ((((c) >> 8) & 0xFF) * (k))), min(240, (int32_t) ((((c) >> 16) & 0xFF) * (k))))

#define COLOR_PACK(r, g, b) color_pack((r), (g), (b))

#define COLOR_BLACK color_pack(0, 0, 0)
#define COLOR_WHITE color_pack(255, 255, 255)

#define COLOR_RED color_pack(255, 0, 0)
#define COLOR_GREEN color_pack(0, 255, 0)
#define COLOR_BLUE color_pack(0, 0, 255)

#define COLOR_YELLOW color_pack(255, 255, 0)

#define IN_INTERVAL(x, a, b) (((x) <= (b)) && ((x) >= (a)))

#define VTOP(v) Point((v).x, (v).y, v.z)

template <class T>
struct Vec2 {
    union {
        struct {T u, v;};
        struct {T x, y;};
        T raw[2];
    };
    Vec2() : u(0), v(0) {}
    Vec2(T _u, T _v) : u(_u),v(_v) {}
    inline Vec2<T> operator +(const Vec2<T> &V) const { return Vec2<T>(u+V.u, v+V.v); }
    inline Vec2<T> operator -(const Vec2<T> &V) const { return Vec2<T>(u-V.u, v-V.v); }
    inline Vec2<T> operator *(float f)          const { return Vec2<T>(u*f, v*f); }
};

template <class T>
struct Vec3 {
    union {
        struct {T x, y, z;};
        struct { T ivert, iuv, inorm; };
        T raw[3];
    };
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(T _x, T _y, T _z) : x(_x),y(_y),z(_z) {}
    Vec3(const Vec3<T> &v) { x = v.x; y = v.y; z = v.z; }
    
    inline Vec3<T> operator ^(const Vec3<T> &v) const { return Vec3<T>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
    inline Vec3<T> operator +(const Vec3<T> &v) const { return Vec3<T>(x+v.x, y+v.y, z+v.z); }
    inline Vec3<T> operator -(const Vec3<T> &v) const { return Vec3<T>(x-v.x, y-v.y, z-v.z); }
    inline Vec3<T> operator *(double f)          const { return Vec3<T>(x*f, y*f, z*f); }
    inline T       operator *(const Vec3<T> &v) const { return x*v.x + y*v.y + z*v.z; }
    double norm() const { return std::sqrt(x*x+y*y+z*z); }
    Vec3<T> &normalize(T l = 1)
    {
        double n = norm();
        if (n < 0.001)
            return *this;
        
        *this = (*this) * (l / n); return *this;
    }
    
    Vec3<T> *clone()
    {
        return new Vec3<T>(x, y, z);
    }
    
    Vec2<T> toVec2()
    {
        return Vec2<T>(x, y);
    }
    
    void to_string(char *str)
    {
        std::cout << str << " (" << x << ", " << y << ", " << z << ")" << std::endl;
    }
};

typedef Vec2<double> Vec2d;
typedef Vec2<int>   Vec2i;
typedef Vec3<double> Vec3d;
typedef Vec3<int>   Vec3i;

/* util func */
// Vec3i dtovec3i(Point d);

struct Point
{
    Point(double x, double y, double z) : color(COLOR_BLACK)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1;
        
        this->u = 0;
        this->v = 0;
        
        this->color = COLOR_BLACK;
    
        this->norm = nullptr;
    }
    
    Point(double x, double y, double z, double u, double v) : color(COLOR_BLACK)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1;
    
        this->u = u;
        this->v = v;
    
        this->norm = nullptr;
    }
    
    Point(double x, double y, double z, color_t color) : color(color)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1;
        
        this->u = 0;
        this->v = 0;
        
        this->norm = nullptr;
    }
    
    Point() : x(0), y(0), z(0), w(1), u(0), v(0), color(COLOR_BLACK), norm(nullptr) {};
    
    ~Point()
    {}
    
    Point(const Point &p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
        this->w = p.w;
        
        this->u = p.u;
        this->v = p.v;
        
        this->color = p.color;
        this->norm = p.norm;
    }
    
    double x;
    double y;
    double z;
    double w;
    
    double u;
    double v;
    
    color_t color;
    
    Vec3d *norm;
    
    void set_normal(Vec3d *n)
    {
        this->norm = n;
    }
    
    void set_uv(double u, double v)
    {
        this->u = u;
        this->v = v;
    }
    
    struct Point operator -(const Point &point)
    {
        Point temp;
        temp.x = this->x - point.x;
        temp.y = this->y - point.y;
        temp.z = this->z - point.z;
        temp.w = 1;
        return temp;
    }
    
    struct Point operator +(const Point &point)
    {
        Point temp;
        temp.x = this->x + point.x;
        temp.y = this->y + point.y;
        temp.z = this->z + point.z;
        temp.w = 1;
        return temp;
    }
    
    struct Point operator *(const double &point)
    {
        Point temp;
        temp.x = this->x * point;
        temp.y = this->y * point;
        temp.z = this->z * point;
        temp.w = 1;
        return temp;
    }
    
    bool operator ==(const Point &point)
    {
        return !(point.x != this->x ||
            point.y != this->y ||
            point.z != this->z);
    }
    
    void normalize()
    {
        double L = sqrt(x * x + y * y + z * z);
        x /= L;
        y /= L;
        z /= L;
    }
};

class Line
{
public:
    Line(Point v1, Point v2);
    Line(Point v1, Point v2, color_t color);
    ~Line();
    
    Line(const Line &l);

    Point v1, v2;
    color_t color;
};

void multi_vect(const Vec3d &A, const Vec3d &B, Vec3d &out);

void set_normal(Vec3d &vec, ...);
void get_normal(Point &p1, Point &p2, Point &p3, Vec3d &out);

double clamp(double a, double left, double right);

bool space(char c);
bool notspace(char c);

std::vector<std::string> split(const std::string& s);

#endif //CURSFIN_UTIL_H
