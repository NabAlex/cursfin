#ifndef CURSFIN_MODEL_H
#define CURSFIN_MODEL_H

#include "util.h"
#include "texture.h"

class Model
{
private:
    vector <Point> vertex;
    vector <vector<int>> connections;
    
//    vector <Point> tex_vertex;
//    vector <vector<int>> tex_connections;
    
    Texture *texture;
public:
    Model() = default;
    Model(Point v1, Point v2, Point v3)
    {
        this->addVertex(v1);
        this->addVertex(v2);
        this->addVertex(v3);
        this->texture = nullptr;
    }
    
    Model(Point v1, Point v2, Point v3, Texture *texture)
    {
        this->addVertex(v1);
        this->addVertex(v2);
        this->addVertex(v3);
        this->texture = texture;
    }
    
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
    
    inline Point getNode(int index)
    {
        return vertex[index];
    }
    
    inline vector<int> getConnect(int index)
    {
        return connections[index];
    }
    
    Texture *getTexture()
    {
        return texture;
    }
    
    size_t sizeConnections()
    {
        return connections.size();
    }
};

#endif //CURSFIN_MODEL_H
