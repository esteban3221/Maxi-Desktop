#include "usuario_historial.hpp"

UsuarioHistorial::UsuarioHistorial()
{

}

UsuarioHistorial::~UsuarioHistorial()
{
}

Glib::RefPtr<Gio::ListStore<MUsuarioHistorial>> UsuarioHistorial::get_usuario_historial()
{
    auto m_list = Gio::ListStore<MUsuarioHistorial>::create();

    return m_list;
}
