#include <gdkmm.h>
#include "scen_controller.h"

Point eye(0, 20, 5), center(10, 20, 5), up(0, 0, -1);

SceneController::SceneController(int width, int height) : BaseDrawer(width, height), render(this), cr(nullptr)
{
    set_size_request(width, height);
    camera = new Camera(eye, center, up, M_PI / 3, M_PI / 3, 50, 0.01, this->width, this->height);
    
    render.set_property(50, 1);
    
    this->insert_mode = false;
}

SceneController::~SceneController()
{
    delete camera;
}

bool SceneController::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    double padding_y = 0;
    /* init scen */
    if (insert_mode)
    {
        padding_y = 40;
        
        cr->set_source_rgb(0., 0., 0.);
        cr->rectangle(0, 0, width, padding_y);
        cr->fill();
    
        cr->set_source_rgb(0., 1., 0.);
        cr->select_font_face("Sans", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
        cr->set_font_size(20.);
        cr->move_to(0, padding_y / 2);
        cr->show_text(this->message);
        cr->set_line_width(1);
        cr->stroke();
    }
    /*************/
    
    this->on_update();
    render.update(this->camera);

    Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_data((const guint8 *) this->frame_buffer,
        Gdk::Colorspace::COLORSPACE_RGB,
        true, 8,
        this->width, this->height, this->width * 4);

    Gdk::Cairo::set_source_pixbuf(cr, pixbuf, 0, padding_y);

    cr->paint();
    
    refresh_frame();
    return true;
}

void SceneController::on_motion_notify(GdkEventMotion *event)
{}

void SceneController::set_insert_mode(bool flag)
{
    this->insert_mode = flag;
    
    if (flag)
        this->message = "";
}

bool SceneController::get_insert_mode()
{
    return this->insert_mode;
}

bool SceneController::on_key_press(GdkEventKey *event)
{
    if (insert_mode)
    {
        if (ONLY_PRESS(event, GDK_KEY_Return))
        {
            this->insert_cmd(this->message);
            insert_mode = false;
        }
        else
        if (ONLY_PRESS(event, GDK_KEY_BackSpace))
        {
            message = message.substr(0, message.size() - 1);
        }
        else
        {
            this->message += event->keyval;
        }
    
        queue_draw();
        return false;
    }
    
    /* insert mode */
    if (ONLY_PRESS(event, GDK_KEY_i))
    {
        set_insert_mode(!insert_mode);
        queue_draw();
        return false;
    }
    
    return true;
}