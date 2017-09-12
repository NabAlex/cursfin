#ifndef CURSFIN_UTIL_H
#define CURSFIN_UTIL_H

#include <vector>
#include "math.h"

using namespace std;

struct Dot {
    Dot(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Dot() : x(0), y(0), z(0), w(1) {};
    double x;
    double y;
    double z;
    double w = 1;
    struct Dot operator - (const Dot &point) {
        Dot temp;
        temp.x = this->x - point.x;
        temp.y = this->y - point.y;
        temp.z = this->z - point.z;
        temp.w = 1;
        return temp;
    }
    struct Dot operator + (const Dot &point) {
        Dot temp;
        temp.x = this->x + point.x;
        temp.y = this->y + point.y;
        temp.z = this->z + point.z;
        temp.w = 1;
        return temp;
    }
    struct Dot operator * (const double &point) {
        Dot temp;
        temp.x = this->x * point;
        temp.y = this->y * point;
        temp.z = this->z * point;
        temp.w = 1;
        return temp;
    }
    
    bool operator == (const Dot &point) {
        if (
            point.x != this->x ||
                point.y != this->y ||
                point.z != this->z
            ) return false;
        return true;
    }
    
    
    
    void normalize() {
        double L = sqrt(x*x+y*y+z*z);
        x /= L;
        y /= L;
        z /= L;
    }
};

class Model
{
private:
    vector <Dot> Vertex;
    vector <Dot> TexVertex;
    vector <vector<int>> Connections;
    vector <vector<int>> TexConnections;
public:
    Model() = default;
    ~Model();
    
    vector <Dot>* getVertex() {
        return &Vertex;
    }
    vector <Dot>* getTexVertex() {
        return &TexVertex;
    }
    
    vector <vector<int>>* getConnections() {
        return &Connections;
    }
    
    vector <vector<int>>* getTexConnections() {
        return &TexConnections;
    }
};

class Color
{
public:
    double r, g, b;
    Color(int r, int g, int b, int a);
};

#endif //CURSFIN_UTIL_H
