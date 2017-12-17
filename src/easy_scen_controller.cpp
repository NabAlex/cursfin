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
    
    Light *light = new Light(10, 10, 10);
    render.add_light(light);
    
//    Vec3d out;
//    Point p1(0, 0, 0), p2(10, 0, 0), p3(0, 0, 10);
//    get_normal(p1, p2, p3, out);
//    render.add_line(Line(p1, VTOP(out), COLOR_GREEN));
    
    double v = 0., u = 0., du = 0.001, dv = 0.001;
    double z = -4;

    Texture *texture = new StoneTexture(1024, 256);

    Point first(-10, 0, 0, u += du, v += dv);
    for (int32_t x = -9; x <= 9; x += 1)
    {
        double y = -sqrt(100 - x * x);
        Point last(x, 0, y, u += du, v += dv);
        Point byZ(x, z, y);
        
        Vec3d n;
        get_normal(first, last, byZ, n);
        first.norm = n.clone();
        last.norm = n.clone();
        byZ.norm = n.clone();
    
        Model *new_model = new Model(first, last, byZ, texture);
        render.add_model(new_model);

//        new_model = new Model(first, Point(last.x, z, last.z, u += du, v += dv), Point(first.x, z, first.z, u += du, v += dv), texture);
//        render.add_model(new_model);

        first = last;
    }
    
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