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
    
    virtual bool on_key_press(GdkEventKey *event);
    virtual void on_motion_notify(GdkEventMotion *event);
    
    virtual void on_update() = 0;
    virtual void insert_cmd(std::string message) = 0;
    
    void set_insert_mode(bool flag);
    bool get_insert_mode();
    
    Camera *camera;
    Renderer render;
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
private:
    const Cairo::RefPtr<Cairo::Context> *cr;
    bool insert_mode;
    
    std::string message;
};

#endif //CURSFIN_DRAW_H
