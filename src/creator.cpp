#include <creator.h>

static Point generate_vertex()
{
    return Point(rand() % 10, rand() % 10, rand() % 10);
}

Creator::Creator(unsigned int seed)
{
    srand(seed);
}

Creator::~Creator()
{}

void Creator::Generate()
{
    Model *raw = new Model();
    raw->addVertex(generate_vertex());
    raw->addVertex(generate_vertex());
    raw->addVertex(generate_vertex());
    
    int c[] = {0, 1, 2};
    raw->addConnection(c, 3);
    this->m = raw;
    
    generated = true;
}

Model *Creator::GetModel()
{
    return this->m;
}

bool Creator::isGenerated()
{
    return this->generated;
}
