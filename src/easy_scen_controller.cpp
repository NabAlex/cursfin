#include "easy_scen_controller.h"

size_t polygons = 20;

EasySceneController::EasySceneController(int width, int height) : SceneController(width, height), creator(polygons, time(0))
{
    this->add_events( Gdk::KEY_PRESS_MASK );
    this->signal_key_release_event().connect ( sigc::mem_fun(*this,
        &EasySceneController::on_key_press) );
    
    
    creator.Generate();
    Model **m = creator.GetModels();
    for (int i = 0; i < creator.SizeModels(); ++i)
        render.add_model(m[i]);
    
    Line lineX(Point(0, 0, 0), Point(10, 0, 0), COLOR_RED);
    Line lineY(Point(0, 0, 0), Point(0, 10, 0), COLOR_GREEN);
    Line lineZ(Point(0, 0, 0), Point(0, 0, 10), COLOR_BLUE);
    render.add_line(lineX);
    render.add_line(lineY);
    render.add_line(lineZ);
}

EasySceneController::~EasySceneController()
{}

void EasySceneController::on_update()
{
    // std::cout << "Update scen (" << width << ", " << height << ")" << std::endl;
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
    
    if (ONLY_PRESS(event, GDK_KEY_j))
        camera->RotateZ(M_PI / 30);
    else
    if (ONLY_PRESS(event, GDK_KEY_k))
        camera->RotateZ(-M_PI / 30);
    
    queue_draw();
    return false;
}