#include "view/pago_a.hpp"

VPago::VPago()
{
    auto builder = Gtk::Builder::create_from_string(View::bloq_num_ui);
    v_base_nip = Gtk::Builder::get_widget_derived<VBaseNip>(builder, "box_nip");
    builder = Gtk::Builder::create_from_string(View::box_colums_ui);

    set_orientation(Gtk::Orientation::VERTICAL);
    append(*v_base_nip);
}

VPago::~VPago()
{
}

// namespace View
// {
//     const char *pagoa_ui = R"()";
// } // namespace View

