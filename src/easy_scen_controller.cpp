#include "easy_scen_controller.h"

Light *light;

EasySceneController::EasySceneController(int width, int height) : SceneController(width, height), creator(time(0))
{
    this->add_events( Gdk::KEY_PRESS_MASK );
    this->signal_key_release_event().connect ( sigc::mem_fun(*this,
        &EasySceneController::on_key_press) );
    
    creator.generate(0, 1);
    creator.generate(10, -1);
    
    for (int i = 0; i < creator.models.size(); ++i)
        render.add_model(creator.models[i]);
    
    Line lineX(Point(0, 0, 0), Point(10, 0, 0), COLOR_RED);
    Line lineY(Point(0, 0, 0), Point(0, 10, 0), COLOR_GREEN);
    Line lineZ(Point(0, 0, 0), Point(0, 0, 10), COLOR_BLUE);
    render.add_line(lineX);
    render.add_line(lineY);
    render.add_line(lineZ);
    
    light = new Light(7);
    
    Point camera_pos = camera->get_me();
    light->set_position(camera_pos);
    light->set_visibility(false);
    render.add_light(light);
}

EasySceneController::~EasySceneController()
{}

void EasySceneController::on_update()
{}

#define ONLY_PRESS(event, button) ((event)->type == GDK_KEY_PRESS && (event)->keyval == (button) && !(event)->state)
bool EasySceneController::on_key_press(GdkEventKey* event)
{
    if (ONLY_PRESS(event, GDK_KEY_w))
        camera->move_on(0, 0, 1);
    else
    if (ONLY_PRESS(event, GDK_KEY_a))
        camera->move_on(-1, 0, 0);
    else
    if (ONLY_PRESS(event, GDK_KEY_d))
        camera->move_on(1, 0, 0);
    else
    if (ONLY_PRESS(event, GDK_KEY_s))
        camera->move_on(0, 0, -1);
    
    if (ONLY_PRESS(event, GDK_KEY_j))
        camera->rotate_z(M_PI / 30);
    else
    if (ONLY_PRESS(event, GDK_KEY_k))
        camera->rotate_z(-M_PI / 30);
    
    if (ONLY_PRESS(event, GDK_KEY_o))
    {
        light->get_model()->v1.x += 1;
        light->x += 1;
    }
    else
    if (ONLY_PRESS(event, GDK_KEY_p))
    {
        light->get_model()->v1.x -= 1;
        light->x -= 1;
    }
    else
    if (ONLY_PRESS(event, GDK_KEY_i))
    {
        light->get_model()->v1.z += 1;
        light->z += 1;
    }
    else
    if (ONLY_PRESS(event, GDK_KEY_u))
    {
        light->get_model()->v1.z -= 1;
        light->z -= 1;
    }
    
    if (ONLY_PRESS(event, GDK_KEY_c))
    {
        light->set_enable(!light->enable);
    }
    Point camera_pos = camera->get_me();
    light->set_position(camera_pos);
    
    queue_draw();
    return false;
}