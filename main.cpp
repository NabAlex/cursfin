#include <gtkmm.h>

#include "scen_controller.h"
#include "easy_scen_controller.h"

class MainWindow : public Gtk::Window
{
public:
    MainWindow()
    {
        add_events(Gdk::KEY_PRESS_MASK);
        add_events(Gdk::POINTER_MOTION_MASK);
    }
    
    ~MainWindow()
    {}
    
    void addController(SceneController &controller)
    {
        this->add(controller);
        controller.show();
        
        controllers.push_back(&controller);
    }
    
    bool on_key_press_event(GdkEventKey *key_event) override
    {
        for (auto c = controllers.begin(); c < controllers.end(); ++c)
            (*c)->on_key_press(key_event);
        
        return Gtk::Window::on_key_press_event(key_event);
    }
    
    bool on_motion_notify_event(GdkEventMotion *event) override
    {
        for (auto c = controllers.begin(); c < controllers.end(); ++c)
            (*c)->on_motion_notify(event);
        
        return Gtk::Window::on_motion_notify_event(event);
    }
private:
    std::vector<SceneController*> controllers;
};

int main(int argc, char *argv[])
{
    const unsigned int w = 800, h = 800;
    
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    
    MainWindow window;
    window.set_default_size(w, h);
    
    EasySceneController main_scene(w, h);
    window.addController(main_scene);

//    int x = 9, y = 9;jjj
//    Gtk::TextView text_view;
//    text_view.set_editable(true);
//    text_view.set_size_request(x, y);
//
//    Gtk::Allocation al = text_view.get_allocation();
//    al.set_width(10);
//    al.set_height(10);
//
//    text_view.set_allocation(al);
//
//
//    text_view.set_size_request(x, y);
//
//    auto buffer = text_view.get_buffer();
//    buffer->set_text("text");
//
//    window.add(text_view);
//
//
//    text_view.show();
    
    
    return app->run(window);
}