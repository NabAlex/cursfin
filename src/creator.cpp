#include <creator.h>

static Dot generate_vertex()
{
    return Dot(rand() % 100, rand() % 100, rand() % 100);
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
}

Model *Creator::GetModel()
{
    return this->m;
}
