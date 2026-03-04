#pragma once
#include <gtkmm.h>
#include <algorithm>
#include "global.hpp"

class VPagoM : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;
protected:
    struct SpinData 
    {
        Gtk::SpinButton* spin;
        int denominacion;
    };

    Gtk::Box *v_box_level_validadores = nullptr;
    Gtk::Button *v_btn_cobrar = nullptr;
    Gtk::Entry *v_ety_concepto = nullptr;

    Gtk::Box *agregar_contenedor(const std::vector<std::pair<size_t,size_t>> &vec_val, std::string key);
    std::map<std::string, std::vector<SpinData>> m_inputs_pago;

    std::unique_ptr<Gtk::MessageDialog> v_dialog;
public:
    VPagoM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VPagoM();
};

namespace View
{
    extern const char *pagom_ui;
} // namespace View