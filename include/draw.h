#ifndef CURSFIN_DRAW_H
#define CURSFIN_DRAW_H

#include <gtkmm/drawingarea.h>

class Draw : public Gtk::DrawingArea
{
public:
public:
    Draw();
    virtual ~Draw();

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    
private:
    void triangle();
};

#endif //CURSFIN_DRAW_H
