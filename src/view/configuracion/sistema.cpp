#include "view/configuracion/sistema.hpp"

VSistema::VSistema(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                            m_builder(refBuilder)
{
    
    v_lbl_sistema[0] = m_builder->get_widget<Gtk::EditableLabel>("ety_info_host");
    v_lbl_sistema[1] = m_builder->get_widget<Gtk::EditableLabel>("ety_info_modelo");
    v_lbl_sistema[2] = m_builder->get_widget<Gtk::EditableLabel>("ety_info_procesador");
    v_lbl_sistema[3] = m_builder->get_widget<Gtk::EditableLabel>("ety_info_memoria");
    v_lbl_sistema[4] = m_builder->get_widget<Gtk::EditableLabel>("ety_info_disco");
    
    v_list_acciones = m_builder->get_widget<Gtk::ListBox>("list_acciones");

    v_lbl_bill[0] = m_builder->get_widget<Gtk::Label>("lbl_data_");
    v_lbl_coin[0] = m_builder->get_widget<Gtk::Label>("lbl_data__");

    v_lbl_titulo_bill = m_builder->get_widget<Gtk::Label>("lbl_titulo");
    v_lbl_titulo_coin = m_builder->get_widget<Gtk::Label>("lbl_titulo_");
    for (size_t i = 0; i < 11; i++)
    {
        v_lbl_bill[i+1] = m_builder->get_widget<Gtk::Label>("lbl_data_" + std::to_string(i));
        v_lbl_coin[i+1] = m_builder->get_widget<Gtk::Label>("lbl_data_" + std::to_string(i) + "_");
    }
    
}

VSistema::~VSistema()
{
}
