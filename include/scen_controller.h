#ifndef CURSFIN_DRAW_H
#define CURSFIN_DRAW_H

#include <assert.h>
#include <gtkmm/drawingarea.h>
#include <iostream>

#include "util.h"
#include "camera.h"
#include "math.h"

#include "base_drawer.h"
#include "renderer.h"

#include "creator.h"

class SceneController : public Gtk::DrawingArea, public BaseDrawer
{
public:
    SceneController(int width, int height);
    ~SceneController();
    
    virtual bool on_key_press(GdkEventKey* event) = 0;
    virtual void on_update() = 0;
    
    Camera *camera;
    Renderer render;
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    
    // void pixie(int x, int y);
private:
    const Cairo::RefPtr<Cairo::Context> *cr;
};

#endif //CURSFIN_DRAW_H
