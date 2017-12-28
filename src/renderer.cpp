#include "renderer.h"

#define CHECK_ZBUFFER() do { if (this->ZBuffer == nullptr) assert(0); } while(0)

void Renderer::ResetBuffer()
{
    for (register int i = 0, N = (drawer->width * drawer->height); i < N; i++)
        ZBuffer[i] = INT_MAX;
}

inline int max(double a, double b, double c)
{
    if (a < b) a = b;
    if (a < c) a = c;
    return int(a + 0.5);
}

inline int min(double a, double b, double c)
{
    if (a > b) a = b;
    if (a > c) a = c;
    return int(a + 0.5);
}

void Renderer::set_property(double far, double near)
{
    this->near = near;
    this->far = far;
}

void Renderer::add_model(Model *model)
{
    this->models.push_back(model);
}

void Renderer::add_rectangle(Point v1, Point v2, Point v3, Point v4)
{
    Model *m1 = new Model(v1, v2, v3), *m2 = new Model(v1, v2, v4);
    this->models.push_back(m1);
    this->models.push_back(m2);
}

void Renderer::add_line(Line line)
{
    this->lines.push_back(line);
}

void Renderer::add_light(Light *light)
{
    this->lights.push_back(light);
}

Renderer::Renderer(BaseDrawer *drawer) : ZBuffer(nullptr)
{
    assert(drawer != nullptr);
    this->drawer = drawer;
    this->near = RENDERER_DEFAULT_NEAR;
    this->far = RENDERER_DEFAULT_FAR;
}

Renderer::~Renderer()
{
    delete[] ZBuffer;
}

Point Renderer::multi_vect(const Point &A, const Point &B) {
    Point Temp;
    Temp.x = A.y * B.z - A.z * B.y;
    Temp.y = A.z * B.x - A.x * B.z;
    Temp.z = A.x * B.y - A.y * B.x;
    return Temp;
}

Point Renderer::barycentric(Point &A, Point &B, Point &C, Point &P) {
    double delta = (B.x - A.x) * (C.y - A.y) - (C.x - A.x) * (B.y - A.y);
    double delta1 = (B.x * C.y - C.x * B.y) + P.x * (B.y - C.y) + P.y * (C.x - B.x);
    double delta2 = (C.x * A.y - A.x * C.y) + P.x * (C.y - A.y) + P.y * (A.x - C.x);
    
    double a = delta1 / delta;
    double b = delta2 / delta;
    
    return Point(a, b, 1. - a - b);
}

void Renderer::draw_line(Point &v1, Point &v2, color_t color)
{
    CHECK_ZBUFFER();
    
    bool steep = false;
    if (std::abs(v1.x - v2.x) < std::abs(v1.y - v2.y))
    {
        std::swap(v1.x, v1.y);
        std::swap(v2.x, v2.y);
        steep = true;
    }
    
    if (v1.x > v2.x) {
        std::swap(v1.x, v2.x);
        std::swap(v1.y, v2.y);
    }
    
    int dx = v2.x - v1.x;
    int dy = v2.y - v1.y;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = v1.y;
    
    double t = 0.;
    double delta = 1 / (v2.x - v1.x + 1);
    for (int x = v1.x; x <= v2.x; x++) {
        double z = (1 - t) * v2.z + t * v1.z;
        
        if (z < ZBuffer[x + y * drawer->width])
        {
            z = ZBuffer[x + y * drawer->width];
            if (steep)
                drawer->pixie(y, x, color);
            else
                drawer->pixie(x, y, color);
        }
        
        error2 += derror2;
        if (error2 > dx) {
            y += (v2.y > v1.y ? 1 : -1);
            error2 -= dx * 2;
        }
        
        t += delta;
    }
}

static inline double get_cos_angle(Vec3d *vec1, Point &p, double x, double y, double z)
{
    Vec3d vec2(x - p.x, y - p.y, z - p.z);
    vec2.normalize();
    
    return vec1->x * vec2.x + vec1->y * vec2.y + vec1->z * vec2.z;
}

static inline double dist(Point &p, double x, double y, double z)
{
    double xx = (p.x - x);
    double yy = (p.y - y);
    double zz = (p.z - z);
    
    return sqrt(xx * xx + yy * yy + zz * zz);
}

void Renderer::draw_triangle(Point &v1, Point &v2, Point &v3,
        double i1, double i2, double i3,
        Texture *texture = nullptr)
{
    CHECK_ZBUFFER();
    
    int MaxX = max(v1.x, v2.x, v3.x);
    int MinX = min(v1.x, v2.x, v3.x);
    int MaxY = max(v1.y, v2.y, v3.y);
    int MinY = min(v1.y, v2.y, v3.y);
    int MaxZ = max(v1.z, v2.z, v3.z);
    int MinZ = min(v1.z, v2.z, v3.z);
    
    if (MaxZ < near || MinZ > far) return;
    if (MaxX < 0 || MaxY < 0 || MinX > drawer->width || MinY > drawer->height) return;
    if (MaxX >= drawer->width) MaxX = drawer->width - 1;
    if (MinX < 0) MinX = 0;
    if (MaxY >= drawer->height) MaxY = drawer->height - 1;
    if (MinY < 0) MinY = 0;

    Point P(MinX, MinY, 1);
    
    double z = 0;
    Point Bary;
    for (int x = MinX; x <= MaxX; x++, P.x++) {
        P.y = MinY;
        for (int y = MinY; y <= MaxY; y++, P.y++) {
            Bary = barycentric(v1, v2, v3, P);
            
            if (Bary.x < 0 || Bary.y < 0 || Bary.z < 0)
                continue;
            
            z = Bary.x * v1.z + Bary.y * v2.z + Bary.z * v3.z;
            if (z >= near && z <= far && z < ZBuffer[x + y * drawer->width]) {
                ZBuffer[x + y * drawer->width] = z;
                
                if (texture != nullptr)
                {
                    double x_color = Bary.x * v1.u + Bary.y * v2.u + Bary.z * v3.u;
                    double y_color = Bary.x * v1.v + Bary.y * v2.v + Bary.z * v3.v;
                    
                    color_t real_color = texture->get_safe(x_color, y_color);
                    
                    double xi = Bary.x * i1 + (1 - Bary.x) * i2;
                    double yi = Bary.y * i1 + (1 - Bary.y) * i3;
                    
                    double final = Bary.z * yi + (1 - Bary.z) * xi;
                    
                    drawer->pixie(x, y, COLOR_MULTI_SAFE(min, real_color, final));
                    
                    continue;
                }
                
#define GET_BARY_COLOR(f, c1, c2, c3, b1, b2, b3) f((c1)) * (b1) + f((c2)) * (b2) + f(c3) * (b3)
                int32_t final_r = GET_BARY_COLOR(COLOR_UNPACK_R, v1.color, v2.color, v3.color, Bary.x, Bary.y, Bary.z);
                int32_t final_g = GET_BARY_COLOR(COLOR_UNPACK_G, v1.color, v2.color, v3.color, Bary.x, Bary.y, Bary.z);
                int32_t final_b = GET_BARY_COLOR(COLOR_UNPACK_B, v1.color, v2.color, v3.color, Bary.x, Bary.y, Bary.z);
                drawer->pixie(x, y, color_pack(final_r, final_g, final_b));
#undef GET_BARY_COLOR
                
            }
        }
    }
}

bool Renderer::draw_model(Camera *camera, Model *m, bool uselight)
{
    CHECK_ZBUFFER();
    
    if (!m->visibility)
        return false;
    
    Point v1_, v2_, v3_;
    int32_t visibles = 0;
    
    visibles += camera->transform(m->v1, v1_, true) ? 0 : 1;
    visibles += camera->transform(m->v2, v2_, true) ? 0 : 1;
    visibles += camera->transform(m->v3, v3_, true) ? 0 : 1;
    if (visibles >= 3)
        return false;
    
    double i1 = 1, i2 = 1, i3 = 1;
    if (uselight && m->v1.norm && m->v2.norm && m->v3.norm)
    {
        i1 = 0; i2 = 0; i3 = 0;
        for (auto light = lights.begin(); light < lights.end(); ++light)
        {
            Light *l = *light;
            if (!l->enable)
                continue;
            
            const double kq = 2;
            const double kd = 3;
            
            i1 += l->it * kd * get_cos_angle(m->v1.norm, m->v1, l->x, l->y, l->z) / (kq + dist(m->v1, l->x, l->y, l->z));
            i2 += l->it * kd * get_cos_angle(m->v2.norm, m->v2, l->x, l->y, l->z) / (kq + dist(m->v2, l->x, l->y, l->z));
            i3 += l->it * kd * get_cos_angle(m->v3.norm, m->v3, l->x, l->y, l->z) / (kq + dist(m->v3, l->x, l->y, l->z));
        }
    }
    
    this->draw_triangle(v1_, v2_, v3_, fabs(i1), fabs(i2), fabs(i3), m->getTexture());
    return true;
}

void Renderer::update(Camera *camera)
{
    assert(camera != nullptr);
    if (ZBuffer != nullptr)
        delete[] ZBuffer;
    
    ZBuffer = new double[drawer->height * drawer->width];
    ResetBuffer();
    
    bool uselight = is_light();
    
    /* only camera */
    if (uselight && lights.size() == 1 && !lights[0]->enable)
        uselight = false;
        
    for (auto m = models.begin(); m < models.end(); ++m)
    {
        assert(*m != nullptr);
        this->draw_model(camera, *m, uselight);
    }
    
    Point v1_, v2_;
    for (auto l = lines.begin(); l < lines.end(); ++l)
    {
        if (!camera->transform((*l).v1, v1_, false))
            continue;
        
        if (!camera->transform((*l).v2, v2_, false))
            continue;
        
        this->draw_line(v1_, v2_, (*l).color);
    }
    
    for (auto light = lights.begin(); light < lights.end(); ++light)
    {
        assert(*light != nullptr);
        if (!(*light)->visibility)
            continue;
        
        this->draw_model(camera, (*light)->get_model(), false);
    }
}

void Renderer::projection(Point &dot)
{}

bool Renderer::is_light()
{
    return !lights.empty();
}

void Renderer::clear()
{
    models.clear();
    lines.clear();
    lights.clear();
}
