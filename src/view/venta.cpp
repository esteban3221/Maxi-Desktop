#include "view/venta.hpp"

VVenta::VVenta()
{
    auto builder = Gtk::Builder::create_from_file("../ui/menu/componente/bloq_numerico.ui");
    v_base_nip = Gtk::Builder::get_widget_derived<VBaseNip>(builder, "box_nip");

    builder = Gtk::Builder::create_from_file("../ui/menu/componente/caja_total.ui");
    v_box_columns = Gtk::Builder::get_widget_derived<BoxColumns>(builder, "box_comp_superior");

    set_orientation(Gtk::Orientation::VERTICAL);
    v_box_columns->set_halign(Gtk::Align::CENTER);

    append(*v_box_columns);
    append(*v_base_nip);
}

VVenta::~VVenta()
{
}

