#include "scen_controller.h"

static Dot eye(0, 0, 0), center(1, 1, 1), up(0, 0, 1);

ScenController::ScenController() : camera(eye, center, up, 60., 60., 100, 0, 100, 100)
{}

ScenController::~ScenController()
{}

void ScenController::pixie(const Cairo::RefPtr<Cairo::Context> &cr, int x, int y)
{
    cr->rectangle(x, y, 1, 1);
    cr->fill();
}

void ScenController::draw_triangle(const Cairo::RefPtr<Cairo::Context> &cr, Vec2i t0, Vec2i t1, Vec2i t2) {
    if (t0.y == t1.y && t0.y == t2.y)
        return;
    
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    
    if (t1.y > t2.y) std::swap(t1, t2);
    
    int total_height = t2.y - t0.y;
    for (int i = 0; i < total_height; i++) {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
        float alpha = (float) i / total_height;
        float beta  = (float) (i - (second_half ? t1.y - t0.y : 0)) / segment_height;
        Vec2i A =               t0 + (t2 - t0) * alpha;
        Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
        if (A.x > B.x)
            std::swap(A, B);
        
        for (int j = A.x; j <= B.x; j++) {
            this->pixie(cr, j, t0.y + i);
        }
    }
}

bool ScenController::draw_model(const Cairo::RefPtr<Cairo::Context> &cr, Model *m)
{
    for (int i = 0; i < m->sizeConnections(); i++)
    {
        std::vector<int> c = m->getConnect(i);
        
        Vec3i d1, d2, d3;
        d1 = dtovec3i(m->getNode(c[0]));
        d2 = dtovec3i(m->getNode(c[1]));
        d3 = dtovec3i(m->getNode(c[2]));
        this->draw_triangle(cr, d1.toVec2(), d2.toVec2(), d3.toVec2());
    }
    
    return true;
}
