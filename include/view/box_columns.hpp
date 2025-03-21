#pragma once
#include <gtkmm.h>

class BoxColumns : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;
    
public:
    BoxColumns(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~BoxColumns();

    Gtk::Label *v_lbl_columns[3]{nullptr};
    Gtk::Entry *v_ety_columns[3]{nullptr};
};

BoxColumns::BoxColumns(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : m_builder{refBuilder},
                                                                                             Gtk::Box(cobject)
{
    for (size_t i = 0; i < 3; i++)
    {
        this->v_lbl_columns[i] = m_builder->get_widget<Gtk::Label>("lbl_colum" + std::to_string(i));
        this->v_ety_columns[i] = m_builder->get_widget<Gtk::Entry>("ety_colum" + std::to_string(i));
    }
}

BoxColumns::~BoxColumns()
{
}
