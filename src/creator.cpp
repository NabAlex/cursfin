#include <creator.h>

#define WIDTH   4
#define HEIGHT  8

static Point generate_vertex()
{
    return Point(rand() % 10, rand() % 10, rand() % 10);
}

Creator::Creator(size_t max_polygons, unsigned int seed)
{
    models = new Model*[max_polygons * 20];
    this->max_polygons = 0;
    
    srand(seed);
    
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

void Creator::Generate()
{
    // PerlinNoise
    
    Texture *texture = new StoneTexture(1024, 32);
    
    size_t index_polygon = 0;
//    models[index_polygon++] = new Model(
//        Point(0, 0, 0, 0, 0),
//        Point(0, 0, 10, 1, 0),
//        Point(10, 0, 0, 0, 1),
//        texture
//    );
//
//    models[index_polygon++] = new Model(
//        Point(10, 0, 0, 0, 1),
//        Point(0, 0, 10, 1, 0),
//        Point(10, 0, 10, 0, 1),
//        texture
//    );
    
    
    models_size = index_polygon;
    
    /*
    
    for (int j = 0; j < 20; ++j)
    {
        point1 = Point(0, j, 0);
        point2 = Point(1, j + 1, 0);
    
        double z = 0, x = 1;
        
        int ylevel = j;
        int y = 1;
    
        int xvisor = 1, zvisor = 1;
    
        for (int i = 0; i < 20; ++i)
        {
            z += zvisor;
            x += xvisor;
            models[index_polygon++] = next_node(Point(x, ylevel + y, z));
        
            y = (y + 1) % 2;
        
            if (x > this->w)
            {
                xvisor = -1;
            }
        
            if (z > this->h)
            {
                zvisor = -1;
            }
        
            if (x < -this->w)
            {
                xvisor = 1;
            }
        }
        
    }
    */
//    Model *raw = new Model();
//    raw->addVertex(generate_vertex());
//    raw->addVertex(generate_vertex());
//    raw->addVertex(generate_vertex());
//
//    int c[] = {0, 1, 2};
//    raw->addConnection(c, 3);
//    this->m = raw;
    
    generated = true;
}

Model **Creator::GetModels()
{
    return this->models;
}

size_t Creator::SizeModels()
{
    return this->models_size;
}

bool Creator::isGenerated()
{
    return this->generated;
}

void Creator::setProperty(int width, int height)
{
    this->w = width;
    this->h = height;
}
