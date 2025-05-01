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

namespace View
{
    extern const char *box_colums_ui;
} // namespace View