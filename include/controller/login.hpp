#pragma once
#include "view/login.hpp"

class Login : public VLogin
{
private:
    /* data */
public:
    Login(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Login();
};
