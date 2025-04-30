#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VLogin : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;
protected:
    Gtk::PasswordEntry *v_ety_passwd;
    Gtk::Button *v_btn_acceder;
    Gtk::Label *v_lbl_titulo;

public:
    VLogin(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VLogin();
};