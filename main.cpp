#include <gtkmm.h>

#include "scen_controller.h"
#include "easy_scen_controller.h"

class MainWindow : public Gtk::Window
{
public:
    MainWindow()
    {
        add_events(Gdk::KEY_PRESS_MASK);
    }
    
    ~MainWindow()
    {}
    
    void addController(ScenController& controller)
    {
        Gtk::Window::add(controller);
        controllers.push_back(std::shared_ptr<ScenController>(&controller));
    }
    
    bool on_key_press_event(GdkEventKey* key_event)
    {
        for (auto c = controllers.begin(); c < controllers.end(); ++c)
            (*c)->on_key_press(key_event);
        
        return Gtk::Window::on_key_press_event(key_event);
    }
private:
    std::vector<std::shared_ptr<ScenController>> controllers;
};

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    
    MainWindow window;
    window.set_default_size(200, 200);
    
    EasyScenController main_scen;
    window.addController(main_scen);
    main_scen.show();
    
    return app->run(window);
}