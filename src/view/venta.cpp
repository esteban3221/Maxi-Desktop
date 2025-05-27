#include "view/venta.hpp"

VVenta::VVenta()
{
    auto builder = Gtk::Builder::create_from_string(View::bloq_num_ui);
    v_base_nip = Gtk::Builder::get_widget_derived<VBaseNip>(builder, "box_nip");

    builder = Gtk::Builder::create_from_string(View::box_colums_ui);
    v_box_columns = Gtk::Builder::get_widget_derived<BoxColumns>(builder, "box_comp_superior");

    set_orientation(Gtk::Orientation::VERTICAL);
    v_box_columns->set_halign(Gtk::Align::CENTER);

    append(*v_box_columns);
    v_ety_concepto.set_margin_top(10);
    v_ety_concepto.set_placeholder_text("Concepto");
    v_ety_concepto.set_css_classes({"title-1"});
    v_ety_concepto.set_margin_start(100);
    v_ety_concepto.set_margin_end(100);
    v_ety_concepto.set_size_request(-1,100);
    append(v_ety_concepto);
    append(*v_base_nip);
}

VVenta::~VVenta()
{
}

