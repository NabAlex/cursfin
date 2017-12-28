#include "easy_scen_controller.h"

bool enable_mouse = false;
static gdouble x_old = 0;

Light *light;

static void default_set_config(std::map<std::string, int32_t> &config)
{
    config["osi"] = 0;
    
    config["intensity"] = 7;
    
    config["red"] = 150;
    config["green"] = 150;
    config["blue"] = 150;
    
    config["height"] = 10;
    
    config["risetop"] = 5;
    config["risedown"] = 5;
    
    config["octaves"] = 4;
    
    config["quality"] = 1;
}

EasySceneController::EasySceneController(int width, int height) : SceneController(width, height), creator()
{
    default_set_config(config);
    
    this->add_events(Gdk::KEY_PRESS_MASK);
    this->signal_key_release_event().connect (sigc::mem_fun(*this,
        &EasySceneController::on_key_press));
    
    this->pass_render();
}

EasySceneController::~EasySceneController()
{}

void EasySceneController::on_update()
{}

bool EasySceneController::on_key_press(GdkEventKey* event)
{
    if (!SceneController::on_key_press(event))
        return false;
    
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

    if (ONLY_PRESS(event, GDK_KEY_c))
    {
        light->set_enable(!light->enable);
    }

    if (ONLY_PRESS(event, GDK_KEY_m))
    {
        enable_mouse = !enable_mouse;
        x_old = 0;
    }
    
    /* rerender */
    if (ONLY_PRESS(event, GDK_KEY_r))
    {
        render.clear();
        creator.models.clear();
        
        this->pass_render();
    }
    
    Point camera_pos = camera->get_me();
    light->set_position(camera_pos);
    
    queue_draw();
    return false;
}

void EasySceneController::on_motion_notify(GdkEventMotion *event)
{
    if (!enable_mouse)
        return;
    
    gdouble dx = x_old - event->x;
    if (!(dx < 1 && dx > -1) && x_old)
    {
        const static gdouble angle = M_PI / 700;
        camera->rotate_z(angle * dx);
    }
    
    x_old = event->x;
    queue_draw();
}

void EasySceneController::insert_cmd(std::string message)
{
    auto cmd = split(message);
    if (cmd.size() <= 1)
        return;
    
    if (cmd[0] == "add")
    {
        Light *l = new Light(std::stoi(cmd[1]));
        Point pl(std::stoi(cmd[2]), std::stoi(cmd[3]), std::stoi(cmd[4]));
        l->set_position(pl);
        l->set_visibility(true);
        render.add_light(l);
    }
    else
    {
        config[cmd[0]] = std::stoi(cmd[1]);
    }
}

void EasySceneController::pass_render()
{
    creator.update_seed(time(0));
    creator.generate(0, config["risedown"], 0, 1);
    
    creator.update_seed(time(0) + 10);
    creator.generate(config["height"], config["risetop"], 0, -1);
    
    for (int i = 0; i < creator.models.size(); ++i)
        render.add_model(creator.models[i]);
    
    render.add_rectangle(Point(0, 0, 0), Point(0, 0, 10), Point(0, 100, 10), Point(0, 100, 0));
    
    if (config["osi"])
    {
        Line lineX(Point(0, 0, 0), Point(10, 0, 0), COLOR_RED);
        Line lineY(Point(0, 0, 0), Point(0, 10, 0), COLOR_GREEN);
        Line lineZ(Point(0, 0, 0), Point(0, 0, 10), COLOR_BLUE);
        render.add_line(lineX);
        render.add_line(lineY);
        render.add_line(lineZ);
    }
    
    light = new Light(config["intensity"]);
    
    Point camera_pos = camera->get_me();
    light->set_position(camera_pos);
    light->set_visibility(false);
    render.add_light(light);
}
