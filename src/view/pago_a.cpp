#include "view/pago_a.hpp"

VPago::VPago()
{
    auto builder = Gtk::Builder::create_from_string(View::bloq_num_ui);
    v_base_nip = Gtk::Builder::get_widget_derived<VBaseNip>(builder, "box_nip");

    builder = Gtk::Builder::create_from_string(View::box_colums_ui);
    v_box_columns = Gtk::Builder::get_widget_derived<BoxColumns>(builder, "box_comp_superior");

    set_orientation(Gtk::Orientation::VERTICAL);
    v_box_columns->set_halign(Gtk::Align::CENTER);

    append(*v_box_columns);
    append(*v_base_nip);
}

VPago::~VPago()
{
}

// namespace View
// {
//     const char *pagoa_ui = R"()";
// } // namespace View

