#include "usuarios.hpp"

Usuarios::Usuarios(/* args */)
{
}

Usuarios::~Usuarios()
{
}

Glib::RefPtr<Gio::ListStore<MUsuarios>> Usuarios::get_usuarios(const nlohmann::json_abi_v3_12_0::json &json)
{
    auto m_list = Gio::ListStore<MUsuarios>::create();

    for (auto &item : json)
    {
        auto id = item["id"].get<size_t>();
        auto usuario = item["username"].get<std::string>();
        auto password = item["password"].get<std::string>();

        auto m_usuario = MUsuarios::create(id, usuario, password);
        m_list->append(m_usuario);
    }

    return m_list;
}