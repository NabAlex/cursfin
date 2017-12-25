#ifndef CURSFIN_EASY_SCEN_CONTROLLER_H
#define CURSFIN_EASY_SCEN_CONTROLLER_H

#include <gdk/gdkkeysyms.h>

#include "scen_controller.h"
#include "scalar_field.h"

class EasySceneController : public SceneController
{
public:
    EasySceneController(int width, int height);
    virtual ~EasySceneController();
    
    bool on_key_press(GdkEventKey* event);
    void on_motion_notify(GdkEventMotion *event);
    
    void on_update() override;
    void insert_cmd(std::string message) override;
private:
    void pass_render();
    
    Creator creator;
};

#endif //CURSFIN_EASY_SCEN_CONTROLLER_H
