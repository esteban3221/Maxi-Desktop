#include "usuarios_roles.hpp"

UsuariosRoles::UsuariosRoles(/* args */)
{
}

UsuariosRoles::~UsuariosRoles()
{
}

Glib::RefPtr<Gio::ListStore<MUsuariosRoles>> UsuariosRoles::get_usuario_roles_by_id(const nlohmann::json_abi_v3_11_3::json &json)
{
    auto m_list = Gio::ListStore<MUsuariosRoles>::create();

    for (auto &item : json)
    {
        auto id = item["id"].get<size_t>();
        auto id_usuario = item["id_usuario"].get<size_t>();
        auto id_rol = item["id_rol"].get<uint16_t>();

        auto m_usuario_roles = MUsuariosRoles::create(id, id_usuario, id_rol);
        m_list->append(m_usuario_roles);
    }
    
    return m_list;
}
