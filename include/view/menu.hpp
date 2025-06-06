#pragma once
#include <gtkmm.h>
#include "global.hpp"

#include "controller/venta.hpp"
#include "controller/pago_a.hpp"
#include "controller/pago_m.hpp"
#include "controller/refill.hpp"
#include "controller/movimientos.hpp"

#include "view/configuracion.hpp"

class VMenu : public Gtk::Box
{
protected:
    Gtk::ListBox *v_list_box_menu;
    Gtk::Stack *v_stack_menu;

private:
    Glib::RefPtr<Gtk::Builder> m_builder;
    void on_menu_item_clicked(Gtk::ListBoxRow *row);
public:
    VMenu(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VMenu();
};

namespace View
{
    extern const char *menu_ui;    
}// namespace View
