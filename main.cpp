#include <gtkmm.h>

#include <draw.h>

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv,
            "org.gtkmm.example");
    
    Gtk::Window window;
    window.set_default_size(200, 200);
    
    Draw area;
    window.add(area);
    area.show();
    
    return app->run(window);
}