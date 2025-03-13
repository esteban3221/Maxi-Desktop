#include "usuarios_roles.hpp"

UsuariosRoles::UsuariosRoles(/* args */)
{
}

UsuariosRoles::~UsuariosRoles()
{
}

Glib::RefPtr<Gio::ListStore<MUsuariosRoles>> UsuariosRoles::get_usuario_roles_by_id(size_t id)
{
    auto m_list = Gio::ListStore<MUsuariosRoles>::create();


    return m_list;
}

void UsuariosRoles::update_usuario_roles(size_t id, const Glib::RefPtr<Gio::ListStore<MRoles>> & list_roles)
{

}
