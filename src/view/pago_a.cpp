#include "view/pago_a.hpp"

VPago::VPago()
{
    #ifdef _WIN32
    auto builder = Gtk::Builder::create_from_string(View::bloq_num_ui);
    #else
    auto builder = Gtk::Builder::create_from_file("../ui/menu/componente/bloq_numerico.ui");
    #endif
    v_base_nip = Gtk::Builder::get_widget_derived<VBaseNip>(builder, "box_nip");

    #ifdef _WIN32
    builder = Gtk::Builder::create_from_string(View::box_colums_ui);
    #else
    builder = Gtk::Builder::create_from_file("../ui/menu/componente/caja_total.ui");
    #endif
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

