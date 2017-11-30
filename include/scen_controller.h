#ifndef CURSFIN_DRAW_H
#define CURSFIN_DRAW_H

#include <gtkmm/drawingarea.h>
#include <iostream>

#include "util.h"
#include "camera.h"

#include "creator.h"

class ScenController : public Gtk::DrawingArea {
public:
    ScenController();
    ~ScenController();
    
    virtual bool on_key_press(GdkEventKey* event) = 0;
    
    void pixie(const Cairo::RefPtr<Cairo::Context> &cr, int x, int y);
    void draw_triangle(const Cairo::RefPtr<Cairo::Context> &cr, Vec2i t0, Vec2i t1, Vec2i t2);
    
    bool draw_model(const Cairo::RefPtr<Cairo::Context> &cr, Model *m);
    
    Camera camera;
};

#endif //CURSFIN_DRAW_H
