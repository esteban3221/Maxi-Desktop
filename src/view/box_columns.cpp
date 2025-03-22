#include "view/box_columns.hpp"

BoxColumns::BoxColumns(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : m_builder{refBuilder},
                                                                                             Gtk::Box(cobject)
{
    for (size_t i = 0; i < 3; i++)
    {
        this->v_lbl_columns[i] = m_builder->get_widget<Gtk::Label>("lbl_colum" + std::to_string(i+1));
        this->v_ety_columns[i] = m_builder->get_widget<Gtk::Entry>("ety_colum" + std::to_string(i+1));
    }
}

BoxColumns::~BoxColumns()
{
}
