#ifndef CURSFIN_UTIL_H
#define CURSFIN_UTIL_H

#include <vector>
#include <math.h>

using namespace std;

/* work with COLOR */
#define COLOR_EXPAND(obj)   (obj).r, (obj).g, (obj).b

#define COLOR_BLACK         0.0, 0.0, 0.0
#define COLOR_RED           1.0, 0.0, 0.0
#define COLOR_GREEN         0.0, 1.0, 0.0
#define COLOR_BLUE          0.0, 0.0, 1.0
#define COLOR_WHITE         1.0, 1.0, 1.0

/* COLOR */

struct Color
{
    Color(double r, double g, double b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    
    Color(const Color &c)
    {
        this->r = c.r;
        this->g = c.g;
        this->b = c.b;
    }
    
    Color()
    {
        this->r = 0.0;
        this->g = 0.0;
        this->b = 0.0;
    }
    
    Color *clone()
    {
        return new Color(*this);
    }
    
    double r;
    double g;
    double b;
};

struct Point
{
    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1;
    }
    
    Point(double x, double y, double z, Color *color)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1;
    }
    
    Point() : x(0), y(0), z(0), w(1) {};
    
    ~Point()
    {}
    
    Point(const Point &p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
        this->w = p.w;
    }
    
    double x;
    double y;
    double z;
    double w;
    
    struct Point operator -(const Point &point) {
        Point temp;
        temp.x = this->x - point.x;
        temp.y = this->y - point.y;
        temp.z = this->z - point.z;
        temp.w = 1;
        return temp;
    }
    
    struct Point operator +(const Point &point) {
        Point temp;
        temp.x = this->x + point.x;
        temp.y = this->y + point.y;
        temp.z = this->z + point.z;
        temp.w = 1;
        return temp;
    }
    struct Point operator *(const double &point) {
        Point temp;
        temp.x = this->x * point;
        temp.y = this->y * point;
        temp.z = this->z * point;
        temp.w = 1;
        return temp;
    }
    
    bool operator ==(const Point &point) {
        return !(point.x != this->x ||
            point.y != this->y ||
            point.z != this->z);
    }
    
    
    
    void normalize() {
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
    Line(Point v1, Point v2, Color color);
    ~Line();
    
    Line(const Line &l);

    Point v1, v2;
    Color color;
};

class Model
{
private:
    vector <Point> vertex;
    vector <vector<int>> connections;
    
    vector <Point> tex_vertex;
    vector <vector<int>> tex_connections;
public:
    Model() = default;
    ~Model() {}
    
    bool addVertex(Point dot)
    {
        this->vertex.push_back(dot);
        return true;
    }
    
    bool addConnection(vector<int> &connect)
    {
        this->connections.push_back(connect);
        return true;
    }
    
    bool addConnection(int *indexes, size_t indexes_len)
    {
        vector<int> v;
        v.reserve(indexes_len);
        for (int i = 0; i < indexes_len; i++)
            v.push_back(indexes[i]);
        
        this->connections.push_back(v);
        return true;
    }
    
    vector<Point>* getVertex()
    {
        return &vertex;
    }
    
    vector<vector<int>>* getConnections()
    {
        return &connections;
    }
    
    vector<Point>* getTexVertex()
    {
        return &tex_vertex;
    }
    
    vector<vector<int>>* getTexConnections()
    {
        return &tex_connections;
    }
    
    inline Point getNode(int index)
    {
        return vertex[index];
    }
    
    inline vector<int> getConnect(int index)
    {
        return connections[index];
    }
    
    size_t sizeConnections()
    {
        return connections.size();
    }
};

template <class t> struct Vec2 {
    union {
        struct {t u, v;};
        struct {t x, y;};
        t raw[2];
    };
    Vec2() : u(0), v(0) {}
    Vec2(t _u, t _v) : u(_u),v(_v) {}
    inline Vec2<t> operator +(const Vec2<t> &V) const { return Vec2<t>(u+V.u, v+V.v); }
    inline Vec2<t> operator -(const Vec2<t> &V) const { return Vec2<t>(u-V.u, v-V.v); }
    inline Vec2<t> operator *(float f)          const { return Vec2<t>(u*f, v*f); }
};

template <class t> struct Vec3 {
    union {
        struct {t x, y, z;};
        struct { t ivert, iuv, inorm; };
        t raw[3];
    };
    Vec3() : x(0), y(0), z(0) {}
    Vec3(t _x, t _y, t _z) : x(_x),y(_y),z(_z) {}
    inline Vec3<t> operator ^(const Vec3<t> &v) const { return Vec3<t>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
    inline Vec3<t> operator +(const Vec3<t> &v) const { return Vec3<t>(x+v.x, y+v.y, z+v.z); }
    inline Vec3<t> operator -(const Vec3<t> &v) const { return Vec3<t>(x-v.x, y-v.y, z-v.z); }
    inline Vec3<t> operator *(float f)          const { return Vec3<t>(x*f, y*f, z*f); }
    inline t       operator *(const Vec3<t> &v) const { return x*v.x + y*v.y + z*v.z; }
    float norm () const { return std::sqrt(x*x+y*y+z*z); }
    Vec3<t> & normalize(t l=1) { *this = (*this)*(l/norm()); return *this; }
    
    Vec2<t> toVec2()
    {
        return Vec2<t>(x, y);
    }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

/* util func */
Vec3i dtovec3i(Point d);

#endif //CURSFIN_UTIL_H
