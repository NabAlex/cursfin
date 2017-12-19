#include <creator.h>

#define WIDTH   50
#define HEIGHT  50

static Point generate_vertex()
{
    return Point(rand() % 10, rand() % 10, rand() % 10);
}

Creator::Creator(unsigned int seed)
{
    this->seed = seed;
    
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

void Creator::generate(int32_t level_z, int32_t inv)
{
    Texture *texture = new StoneTexture(1024, 128);
    ScalarField scalar_field(SCALAR_CAVE_RANDOM, this->seed, 0, 5, this->w);
    
    double count_textures = 10;
    double u = 0., v = 0., du = 1. / this->w * count_textures, dv = 1. / this->w * count_textures;
    
    const double ulimit = 1. - (du / 2), vlimit = 1. - (dv / 2);
    double **map = scalar_field.field;
    for (int32_t y = 0; y < this->h - 1; ++y)
    {
        for (int32_t x = 0; x < this->w - 1; ++x)
        {
            /* create triangle */
            /* 2 -- 1          */
            /*  ----           */
            /* 4 -- 3          */
            /*******************/
            Point v1(x, y, level_z + inv * map[x][y], u, v);
            Point v2(x + 1, y, level_z + inv * map[x + 1][y], u, v + dv);
            Point v3(x, y + 1, level_z + inv * map[x][y + 1], u + du, v);
            Point v4(x + 1, y + 1, level_z + inv * map[x + 1][y + 1], u + du, v + dv);
            
            Vec3d vec1, vec2;
            get_normal(v1, v2, v4, vec1);
            set_normal(vec1, v1, v2, v4, NULL);
            
            get_normal(v1, v3, v4, vec2);
            set_normal(vec2, v1, v3, v4, NULL);
            
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
