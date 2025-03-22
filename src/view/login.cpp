#include "view/login.hpp"

VLogin::VLogin(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                        m_builder(refBuilder)
{
    v_ety_passwd = m_builder->get_widget<Gtk::PasswordEntry>("ety_passwd");
    v_btn_acceder = m_builder->get_widget<Gtk::Button>("btn_acceder");
}

VLogin::~VLogin()
{
}