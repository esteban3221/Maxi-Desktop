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
    BoxColumns *v_box_columns = nullptr;

    std::unique_ptr<Gtk::MessageDialog> v_dialog;
public:
    VVenta();
    ~VVenta();
};