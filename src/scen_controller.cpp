#include "scen_controller.h"

Point eye(0, 40, 5), center(0, 0, 5), up(0, 0, -1);

SceneController::SceneController(int width, int height) : BaseDrawer(width, height), render(this), cr(nullptr)
{
    set_size_request(width, height);
    camera = new Camera(eye, center, up, M_PI / 3, M_PI / 3, 50, 0, this->width, this->height);
    
    // render.set_property();
}

SceneController::~SceneController()
{
    delete camera;
}

//void SceneController::pixie(int x, int y)
//{
//
//    assert(cr != nullptr);
////    (*cr)->rectangle(x, y, 1, 1);
////    (*cr)->fill();
//}

bool SceneController::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    cr->set_source_rgb(COLOR_WHITE);
    cr->rectangle(0.0, 0.0, width, height);
    cr->fill();
    cr->save();
    
    /* init scen */
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    cr->set_line_width(1.0);
    /*************/
    
    Gtk::Allocation allocation = get_allocation();
    this->width = allocation.get_width();
    this->height = allocation.get_height();
    this->cr = &cr;
    
    this->on_update();
    render.update(this->camera);
    
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (!matrix[i][j].act)
                continue;
            
            cr->set_source_rgb(COLOR_EXPAND(matrix[i][j].color));
            cr->move_to(i, j);
            cr->close_path();
            cr->stroke(); // TODO (a.naberezhnyi) make faster
        }
    }
    
    refresh_frame();
    return true;
}
