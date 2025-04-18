#include "view/configuracion/impresora.hpp"

VImpresora::VImpresora(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),m_builder(refBuilder)
{
    v_list_box_print = m_builder->get_widget<Gtk::ListBox>("list_box_print");
    v_ntbook_propiedades = m_builder->get_widget<Gtk::Notebook>("ntbookPropiedades");
    v_text_view = m_builder->get_widget<Gtk::TextView>("text_view");

    local.v_lbox_activa = m_builder->get_widget<Gtk::ListBox>("listBoxPage1_");
    local.v_switch_activa = m_builder->get_widget<Gtk::Switch>("switchImpresion_");
    local.v_lbox_vizualizador = m_builder->get_widget<Gtk::ListBox>("listBoxVisualizacion1_");

    for (size_t i = 0; i < 6; i++)
    {
        local.v_check_vizualizador[i] = m_builder->get_widget<Gtk::CheckButton>("checkConfig" + std::to_string(i+1) + "_");
        remoto.v_check_vizualizador[i] = m_builder->get_widget<Gtk::CheckButton>("checkConfig" + std::to_string(i+1));
    }

    local.v_lbox_test = m_builder->get_widget<Gtk::ListBox>("listBoxTest1_");

    remoto.v_lbox_activa = m_builder->get_widget<Gtk::ListBox>("listBoxPage1");
    remoto.v_switch_activa = m_builder->get_widget<Gtk::Switch>("switchImpresion");
    remoto.v_lbox_vizualizador = m_builder->get_widget<Gtk::ListBox>("listBoxVisualizacion1");
    remoto.v_lbox_test = m_builder->get_widget<Gtk::ListBox>("listBoxTest1");
}

VImpresora::~VImpresora()
{
}

VImpresora::VListPrinters::VListPrinters(const std::string &titulo, const std::string &subtitulo)
{
    v_image_printer = Gtk::manage(new Gtk::Image());
    v_image_printer->set_from_icon_name("printer-symbolic");
    v_image_printer->set_icon_size(Gtk::IconSize::LARGE);

    v_image_check = Gtk::manage(new Gtk::Image());
    v_image_check->set_from_icon_name("checkmark-symbolic");
    v_image_check->set_opacity(0);

    v_titulo = Gtk::manage(new Gtk::Label(titulo));
    v_titulo->set_halign(Gtk::Align::START);
    v_titulo->set_css_classes({"title-4"});
    v_subtitulo = Gtk::manage(new Gtk::Label(subtitulo));
    v_subtitulo->set_css_classes({"dim-label"});
    v_subtitulo->set_ellipsize(Pango::EllipsizeMode::END);
    v_subtitulo->set_justify(Gtk::Justification::LEFT);

    auto box_label = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL));
    box_label->append(*v_titulo);
    box_label->append(*v_subtitulo);
    box_label->set_hexpand();

    v_box = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL));
    v_box->append(*v_image_printer);
    v_box->append(*box_label);
    v_box->append(*v_image_check);
    v_image_check->set_halign(Gtk::Align::END);
    v_box->set_margin(5);

    set_child(*v_box);
}