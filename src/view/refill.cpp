#include "view/refill.hpp"
#include "refill.hpp"

VRefill::VRefill(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                          m_builder(refBuilder)
{
    v_tree_reciclador_billetes = m_builder->get_widget<Gtk::ColumnView>("treeRecicladorBilletes");
    v_tree_reciclador_monedas = m_builder->get_widget<Gtk::ColumnView>("treeRecicladorMonedas");

    v_lbl_total_parcial_monedas = m_builder->get_widget<Gtk::Label>("lblTotalMXN1");
    v_lbl_total_parcial_billetes = m_builder->get_widget<Gtk::Label>("lblTotalMXN2");
    v_lbl_total_monedas = m_builder->get_widget<Gtk::Label>("lblTotalMXN4");
    v_lbl_total_billetes = m_builder->get_widget<Gtk::Label>("lblTotalMXN5");
    v_lbl_total = m_builder->get_widget<Gtk::Label>("lblTotalMXN3");

    v_btn_incia = m_builder->get_widget<Gtk::Button>("btn_iniciar");
    
}

void VRefill::on_setup_label(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::END));
}

void VRefill::on_setup_button(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto button = (Gtk::make_managed<Gtk::Button>());
    button->set_icon_name("document-save-symbolic");
    list_item->set_child(*button);
}

void VRefill::on_setup_spin(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto spin = Gtk::make_managed<Gtk::SpinButton>(Gtk::Adjustment::create(0, 0, 100, 1, 10), 1.0, 0);
    spin->set_halign(Gtk::Align::FILL);
    list_item->set_child(*spin);
}

void VRefill::on_bind_deno(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_denominacion));
}

void VRefill::on_bind_alm(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_cant_alm));
}

void VRefill::on_bind_recy(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_cant_recy));
}

void VRefill::on_bind_ingreso(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_ingreso));
}

void VRefill::on_bind_inmo(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
    auto spin = dynamic_cast<Gtk::SpinButton *>(list_item->get_child());

    spin->set_adjustment(Gtk::Adjustment::create(col->m_nivel_inmo,0,100));
}

void VRefill::on_bind_btn(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
    auto button = dynamic_cast<Gtk::Button *>(list_item->get_child());

    button->signal_clicked().connect([this,col](){
        std::cout << "Denominacion: " << col->m_denominacion << '\n';
    });
}


VRefill::~VRefill()
{
}