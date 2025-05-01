#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VEmpresa : public Gtk::Box
{
private:
    /* data */
    Glib::RefPtr<Gtk::Builder> m_builder;
protected:
    Gtk::Entry *v_datos_empresa[5]{nullptr};
    Gtk::Button *v_btn_guardar{nullptr};
public:
    VEmpresa(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VEmpresa();
};

namespace View
{
    namespace Conf
    {
        extern const char *empresa_ui;
    } // namespace Conf
} // namespace View