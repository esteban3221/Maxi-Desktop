#pragma once
#include <gtkmm.h>
#include "global.hpp"
#include "bloq_num.hpp"
#include "box_columns.hpp"

class VVenta : public Gtk::Box
{
private:

protected:
    VBaseNip *v_base_nip = nullptr;
    Gtk::Entry v_ety_concepto;
    std::unique_ptr<Gtk::MessageDialog> v_dialog;
public:
    VVenta();
    ~VVenta();
};