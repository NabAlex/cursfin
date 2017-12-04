#include "easy_scen_controller.h"

EasySceneController::EasySceneController(int width, int height) : SceneController(width, height), creator(time(0))
{
    this->add_events( Gdk::KEY_PRESS_MASK );
    this->signal_key_release_event().connect ( sigc::mem_fun(*this,
        &EasySceneController::on_key_press) );
    
    
    creator.Generate();
    Model *m = creator.GetModel();
    render.add_model(m);
    
    Line lineX(Point(0, 0, 0), Point(10, 0, 0), Color(COLOR_RED));
    Line lineY(Point(0, 0, 0), Point(0, 10, 0), Color(COLOR_GREEN));
    Line lineZ(Point(0, 0, 0), Point(0, 0, 10), Color(COLOR_BLUE));
    render.add_line(lineX);
    render.add_line(lineY);
    render.add_line(lineZ);
}

EasySceneController::~EasySceneController()
{}

void EasySceneController::on_update()
{
    std::cout << "Update scen (" << width << ", " << height << ")" << std::endl;
}

#define ONLY_PRESS(event, button) ((event)->type == GDK_KEY_PRESS && (event)->keyval == (button) && !(event)->state)
bool EasySceneController::on_key_press(GdkEventKey* event)
{
    if (ONLY_PRESS(event, GDK_KEY_w))
        camera->MoveOn(0, 0, 1);
    else
    if (ONLY_PRESS(event, GDK_KEY_a))
        camera->MoveOn(-1, 0, 0);
    else
    if (ONLY_PRESS(event, GDK_KEY_d))
        camera->MoveOn(1, 0, 0);
    else
    if (ONLY_PRESS(event, GDK_KEY_s))
        camera->MoveOn(0, 0, -1);
    
    queue_draw();
    return false;
}