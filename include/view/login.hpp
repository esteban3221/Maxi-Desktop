#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VLogin : public Gtk::Box
{
    protected:
        Glib::RefPtr<Gtk::Builder> m_builder;
        Gtk::PasswordEntry *v_ety_passwd;
        Gtk::Button *v_btn_acceder;
    
    public:
        VLogin(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
        ~VLogin();
};