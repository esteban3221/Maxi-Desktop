#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VSistema : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;
protected:
    Gtk::EditableLabel *v_lbl_sistema[5]{nullptr};
    Gtk::ListBox *v_list_acciones{nullptr};
    Gtk::Label *v_lbl_bill[12]{nullptr} , *v_lbl_titulo_bill{nullptr};
    Gtk::Label *v_lbl_coin[12]{nullptr}, *v_lbl_titulo_coin{nullptr};

public:
    VSistema(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VSistema();
};