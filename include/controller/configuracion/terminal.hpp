#pragma once
#include <gtkmm.h>
#include <memory>

#include "view/configuracion/terminal.hpp"
#include "view/configuracion/c_card_mp.hpp"

class Terminal : public VTerminal
{
private:
    void on_button_add_clicked();
    void on_button_close_rvl_clicked();
public:
    Terminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Terminal();
};