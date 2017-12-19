#ifndef CURSFIN_OBJECT_H
#define CURSFIN_OBJECT_H

class Object
{
public:
    Object() : visibility(true)
    {}
    
    Object(bool flag) : visibility(flag)
    {}
    
    void set_visibility(bool flag)
    {
        visibility = flag;
    }
    
    bool visibility;
};

#endif //CURSFIN_OBJECT_H
