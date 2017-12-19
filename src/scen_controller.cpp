#include <gdkmm.h>
#include "scen_controller.h"

Point eye(0, 20, 5), center(0, 0, 5), up(0, 0, -1);

SceneController::SceneController(int width, int height) : BaseDrawer(width, height), render(this), cr(nullptr)
{
    set_size_request(width, height);
    camera = new Camera(eye, center, up, M_PI / 3, M_PI / 3, 50, 1, this->width, this->height);
    
    render.set_property(50, 1);
    // render.set_property();
}

SceneController::~SceneController()
{
    delete camera;
}

bool SceneController::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    
    /* init scen */
    
    /*************/
    
    this->on_update();
    render.update(this->camera);

    Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_data((const guint8 *) this->frame_buffer,
        Gdk::Colorspace::COLORSPACE_RGB,
        true, 8,
        this->width, this->height, this->width * 4);
    
    cr->save();

    Gdk::Cairo::set_source_pixbuf(cr, pixbuf, 0, 0);
    
    cr->paint();
    refresh_frame();
    return true;
}
