#include <creator.h>

#define WIDTH   50
#define HEIGHT  50

static Point generate_vertex()
{
    return Point(rand() % 10, rand() % 10, rand() % 10);
}

Creator::Creator()
{
    this->seed = time(0);
    
    this->w = WIDTH;
    this->h = HEIGHT;
}

Creator::~Creator()
{}

static Point point1, point2;
static Model *next_node(Point new_point)
{
    Model *m = new Model(point1, point2, new_point);
    point2 = point1;
    point1 = new_point;
    return m;
}

void Creator::generate(int32_t level_z,
        int32_t rise,
        int32_t down,
        int32_t inv)
{
    Texture *texture = new StoneTexture(1024, 128);
    ScalarField scalar_field(SCALAR_CAVE_RANDOM, this->seed, -down, rise, this->w);
    
    double count_textures = 10;
    double u = 0., v = 0., du = 1. / this->w * count_textures, dv = 1. / this->w * count_textures;
    
    const double ulimit = 1. - (du / 2), vlimit = 1. - (dv / 2);
    double **map = scalar_field.field;
    
    double base = 1 / (double) config["quality"];
    for (int32_t y = 0; y < this->h - 1; ++y)
    {
        for (int32_t x = 0; x < this->w - 1; ++x)
        {
            /* create triangle */
            /* 2 -- 1          */
            /*  ----           */
            /* 4 -- 3          */
            /*******************/
            
            double xbase = x * base, ybase = y * base;
            double x1base = (x + 1) * base, y1base = (y + 1) * base;
            
            Point v1(xbase, ybase, level_z + inv * map[x][y], u, v);
            Point v2(x1base, ybase, level_z + inv * map[x + 1][y], u, v + dv);
            Point v3(xbase, y1base, level_z + inv * map[x][y + 1], u + du, v);
            Point v4(x1base, y1base, level_z + inv * map[x + 1][y + 1], u + du, v + dv);
            
            Vec3d vec1, vec2, vec3, vec4;
            get_point_normal(scalar_field, x, y, base, vec1);
            get_point_normal(scalar_field, x + 1, y, base, vec2);
            get_point_normal(scalar_field, x, y + 1, base, vec3);
            get_point_normal(scalar_field, x + 1, y + 1, base, vec4);
            
            set_normal(vec1, v1, NULL);
            set_normal(vec2, v2, NULL);
            set_normal(vec3, v3, NULL);
            set_normal(vec4, v4, NULL);
    
            models.push_back(new Model(v1, v2, v4, texture));
            models.push_back(new Model(v1, v3, v4, texture));
    
            v += dv;
            if (v >= vlimit)
                v = 0.;
        }
        
        v = 0.;
    
        u += du;
        if (u >= ulimit)
            u = 0.;
    }

    generated = true;
}

bool Creator::is_generated()
{
    return this->generated;
}

void Creator::set_property(int width, int height)
{
    this->w = width;
    this->h = height;
}

void Creator::update_seed(unsigned int seed)
{
    this->seed = seed;
}

void Creator::get_point_normal(ScalarField &scalarField,
        int32_t x, int32_t y,
        double base,
        Vec3d &out)
{
    if (x <= 0 || y <= 0 || x >= this->w - 1 || y >= this->w - 1)
    {
        out = Vec3d();
        return;
    }
    
    double xbase = x * base; double ybase = y * base;
    
    Point v = Point(xbase, ybase, scalarField.field[x][y]);
    Point v1 = Point((x + 1) * base, y * base, scalarField.field[x + 1][y]);
    Point v2 = Point((x - 1) * base, y * base, scalarField.field[x - 1][y]);
    
    Point v3 = Point(x * base, (y + 1) * base, scalarField.field[x][y + 1]);
    Point v4 = Point(x * base, (y - 1) * base, scalarField.field[x][y - 1]);
    
    Vec3d vec12, vec23, vec34, vec14;
    get_normal(v, v1, v2, vec12);
    get_normal(v, v2, v3, vec23);
    get_normal(v, v3, v4, vec34);
    get_normal(v, v1, v4, vec14);
    
    out = (vec12 + vec23 + vec34 + vec14) * ((double) 1 / 4);
}
