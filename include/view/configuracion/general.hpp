#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VGeneral : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;
protected:
    Gtk::Entry *v_ety_mensaje_inicio;
    Gtk::Button *v_btn_reinicia_val;
    Gtk::Button *v_btn_actualiza_pos;
    
    std::unique_ptr<Gtk::MessageDialog> v_dialog;
public:
    VGeneral(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VGeneral();
};

namespace View
{
    namespace Conf
    {
        extern const char *general_ui;
    } // namespace Conf
} // namespace View