#pragma once
#include <iostream>
#include <glibmm.h>
#include <giomm.h>
#include <memory>
#include "global.hpp"


class MUsuarios : public Glib::Object
{
public:
    size_t m_id;
    Glib::ustring m_usuario;
    Glib::ustring m_passsword;

    static Glib::RefPtr<MUsuarios> create(size_t id, const Glib::ustring &usuario, const Glib::ustring &password)
    {
        return Glib::make_refptr_for_instance<MUsuarios>(new MUsuarios(id, usuario, password));
    }

protected:
    MUsuarios(uint16_t id, const Glib::ustring &usuario, const Glib::ustring &password)
        : m_id(id), m_usuario(usuario), m_passsword(password)
    {
    }
};

class Usuarios
{
private:
    /* data */
public:
    Usuarios(/* args */);
    ~Usuarios();

    Glib::RefPtr<Gio::ListStore<MUsuarios>> get_usuarios(const nlohmann::json_abi_v3_12_0::json &json);
};