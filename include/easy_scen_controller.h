#ifndef CURSFIN_EASY_SCEN_CONTROLLER_H
#define CURSFIN_EASY_SCEN_CONTROLLER_H

#include <gdk/gdkkeysyms.h>

#include "scen_controller.h"

class EasyScenController : public ScenController
{
public:
    EasyScenController();
    virtual ~EasyScenController();
    
    bool on_key_press(GdkEventKey* event);
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

#endif //CURSFIN_EASY_SCEN_CONTROLLER_H
