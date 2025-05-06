#pragma once
#include <iostream>
#include <glibmm.h>
#include <giomm.h>
#include <memory>


#include "roles.hpp"
#include "global.hpp"

class MUsuariosRoles : public Glib::Object
{
public:
    size_t m_id;
    size_t m_id_usuario;
    uint16_t m_id_rol;

    static Glib::RefPtr<MUsuariosRoles> create(size_t id, size_t id_usuario, uint16_t id_rol)
    {
        return Glib::make_refptr_for_instance<MUsuariosRoles>(new MUsuariosRoles(id, id_usuario, id_rol));
    }

protected:
    MUsuariosRoles(size_t id, size_t id_usuario, uint16_t id_rol)
        : m_id(id), m_id_usuario(id_usuario), m_id_rol(id_rol)
    {
    }
};

class UsuariosRoles
{
private:
    /* data */
public:
    UsuariosRoles(/* args */);
    ~UsuariosRoles();
    
    Glib::RefPtr<Gio::ListStore<MUsuariosRoles>> get_usuario_roles_by_id(const nlohmann::json_abi_v3_12_0::json &json);
};