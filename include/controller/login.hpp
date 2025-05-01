#pragma once
#include "view/login.hpp"
#include "model/configuracion.hpp"

class Login : public VLogin
{
private:
    void on_btn_login_clicked();
    void on_show_map();
public:
    Login(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Login();
};