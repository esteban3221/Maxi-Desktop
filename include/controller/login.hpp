#pragma once
#include "view/login.hpp"

class Login : public VLogin
{
private:
    void on_btn_login_clicked();
public:
    Login(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Login();
};
