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
    
    void addController(SceneController& controller)
    {
        this->add(controller);
        controller.show();
        
        controllers.push_back(std::shared_ptr<SceneController>(&controller));
    }
    
    bool on_key_press_event(GdkEventKey* key_event)
    {
        for (auto c = controllers.begin(); c < controllers.end(); ++c)
            (*c)->on_key_press(key_event);
        
        return Gtk::Window::on_key_press_event(key_event);
    }
private:
    std::vector<std::shared_ptr<SceneController>> controllers;
};

int main(int argc, char *argv[])
{
    const unsigned int w = 800, h = 800;
    
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    
    MainWindow window;
    window.set_default_size(w, h);
    
    EasySceneController main_scen(w, h);
    window.addController(main_scen);
    
    return app->run(window);
}