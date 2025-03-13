#include "model/roles.hpp"

Rol::Rol(/* args */)
{
}

Rol::~Rol()
{
}

Glib::RefPtr<Gio::ListStore<MRoles>> Rol::get_roles()
{
    auto m_list = Gio::ListStore<MRoles>::create();


    return m_list;
}