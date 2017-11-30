#include "easy_scen_controller.h"

EasyScenController::EasyScenController() : ScenController()
{
    this->add_events( Gdk::KEY_PRESS_MASK );
    this->signal_key_release_event().connect ( sigc::mem_fun(*this,
        &EasyScenController::on_key_press) );
//    this->signal_key_press_event().connect(
//        sigc::mem_fun(*this, &EasyScenController::onKeyPress), false
//    );
}

EasyScenController::~EasyScenController()
{
}

#define ONLY_PRESS(event, button) ((event)->type == GDK_KEY_PRESS && (event)->keyval == (button) && !(event)->state)
bool EasyScenController::on_key_press(GdkEventKey* event)
{
    std::cout << "Key: " << event->keyval << std::endl;
    if (ONLY_PRESS(event, GDK_KEY_w))
    {
        std::cout << "test!" << std::endl;
    }
    return false;
}

bool EasyScenController::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    
    std::cout << "Generate size: (" << width << ", " << height << ")" << std::endl;
    
    cr->set_line_width(3.0);
    cr->set_source_rgb(0.8, 0.0, 0.0);
    
    Creator creator(time(0));
    creator.Generate();
    
    camera.SetActive(creator.GetModel());
    
    this->draw_model(cr, creator.GetModel());
    
    return true;
}
