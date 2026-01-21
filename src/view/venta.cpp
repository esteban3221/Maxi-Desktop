#include "view/venta.hpp"

VVenta::VVenta()
{
    auto builder = Gtk::Builder::create_from_string(View::bloq_num_ui);
    v_base_nip = Gtk::Builder::get_widget_derived<VBaseNip>(builder, "box_nip");

    builder = Gtk::Builder::create_from_string(View::box_colums_ui);
    v_box_columns = Gtk::Builder::get_widget_derived<BoxColumns>(builder, "box_comp_superior");

    v_revealer_columns.set_child(*v_box_columns);
    v_revealer_columns.set_reveal_child(false);
    append(v_revealer_columns);
    
    set_orientation(Gtk::Orientation::VERTICAL);
    
    v_ety_concepto.set_margin_top(10);
    v_ety_concepto.set_placeholder_text("Concepto");
    v_ety_concepto.set_css_classes({"title-1"});
    v_ety_concepto.set_margin_start(100);
    v_ety_concepto.set_margin_end(100);
    v_ety_concepto.set_size_request(-1,100);
    Global::Utility::set_multiline_text(v_ety_concepto);
    append(v_ety_concepto);
    append(*v_base_nip);
}

VVenta::~VVenta()
{
}

