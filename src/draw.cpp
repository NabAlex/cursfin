#include "draw.h"

Draw::Draw()
{
}

Draw::~Draw()
{
}

void Draw::triangle()
{
    void triangle(const Cairo::RefPtr<Cairo::Context>& cr, Vec2i t0, Vec2i t1, Vec2i t2) {
        if (t0.y == t1.y && t0.y == t2.y)
            return;
        // sort the vertices, t0, t1, t2 lower-to-upper (bubblesort yay!)
        if (t0.y>t1.y) std::swap(t0, t1);
        if (t0.y>t2.y) std::swap(t0, t2);
        if (t1.y>t2.y) std::swap(t1, t2);
        int total_height = t2.y-t0.y;
        for (int i=0; i<total_height; i++) {
            bool second_half = i>t1.y-t0.y || t1.y==t0.y;
            int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
            float alpha = (float)i/total_height;
            float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
            Vec2i A =               t0 + (t2-t0)*alpha;
            Vec2i B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta;
            if (A.x>B.x) std::swap(A, B);
            for (int j=A.x; j<=B.x; j++) {
                image.set(j, t0.y+i, color); // attention, due to int casts t0.y+i != A.y
            }
        }
    }
}

bool Draw::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    
    // coordinates for the center of the window
    int xc, yc;
    xc = width / 2;
    yc = height / 2;
    
    cr->set_line_width(1.0);
    
    // draw red lines out from the center of the window
    cr->set_source_rgb(0.8, 0.0, 0.0);
    cr->move_to(0, 0);
    cr->line_to(xc, yc);
    cr->line_to(0, height);
    cr->move_to(xc, yc);
    cr->line_to(width, yc);
    cr->stroke();
    
    return true;
}