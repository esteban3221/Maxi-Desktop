#include "view/menu.hpp"

VMenu::VMenu(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject), m_builder(refBuilder)
{

    v_list_box_menu = m_builder->get_widget<Gtk::ListBox>("list_box_menu");
    v_stack_menu = m_builder->get_widget<Gtk::Stack>("stack_menu");
    v_stack_menu->set_transition_type(Gtk::StackTransitionType::SLIDE_UP_DOWN);
    v_list_box_menu->signal_row_activated().connect(sigc::mem_fun(*this, &VMenu::on_menu_item_clicked));

    Global::Widget::v_progress_bar = m_builder->get_widget<Gtk::ProgressBar>("progress");

    v_stack_menu->set_margin(10);
    v_stack_menu->add(* Gtk::manage(new Venta()), "0");
    v_stack_menu->add(* Gtk::manage(new PagoA()), "1");

    auto builder = Gtk::Builder::create_from_file("../ui/menu/pago_m.ui");
    auto pago_m = Gtk::Builder::get_widget_derived<PagoM>(builder, "box_pago_m");
    v_stack_menu->add(*pago_m, "2");

    builder = Gtk::Builder::create_from_file("../ui/menu/refill.ui");
    auto refill = Gtk::Builder::get_widget_derived<Refill>(builder, "box_refill");
    v_stack_menu->add(*refill, "3");

    builder = Gtk::Builder::create_from_file("../ui/menu/movimientos.ui");
    auto movimientos = Gtk::Builder::get_widget_derived<Movimientos>(builder, "box_movimientos");
    v_stack_menu->add(*movimientos, "4");

    v_stack_menu->add(* Gtk::manage(new VConfiguracion()), "5");

    signal_map().connect([this](){v_stack_menu->set_visible_child("0");});
}

VMenu::~VMenu()
{
}

void VMenu::on_menu_item_clicked(Gtk::ListBoxRow * row)
{
    v_stack_menu->set_visible_child(std::to_string(row->get_index()));
}
