#include "usuarios.hpp"

Usuarios::Usuarios(/* args */)
{
}

Usuarios::~Usuarios()
{
}

Glib::RefPtr<Gio::ListStore<MUsuarios>> Usuarios::get_usuarios()
{
    auto m_list = Gio::ListStore<MUsuarios>::create();

    return m_list;
}

const std::pair<int, std::string> Usuarios::existe_usuario(const std::string &pass) const
{

    return {-1 , "sds"};
}

size_t Usuarios::insert_usuario(const Glib::RefPtr<MUsuarios> &usuario)
{
    
    return std::stoull("0");
}

void Usuarios::update_usuario(const Glib::RefPtr<MUsuarios> &usuario)
{
    
}

void Usuarios::delete_usuario(const Glib::RefPtr<MUsuarios> &usuario)
{
    
}
