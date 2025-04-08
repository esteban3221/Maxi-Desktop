#include "model/configuracion.hpp"

Configuracion::Configuracion(/* args */)
{
}

Configuracion::~Configuracion()
{
}

Glib::RefPtr<Gio::ListStore<MConfiguracion>> Configuracion::get_conf_data(int inicio, int fin)  const
{
    auto &database = Database::getInstance();
    database.sqlite3->command("select * from configuracion where id BETWEEN ? AND ? ORDER BY id ASC", inicio, fin);
    auto m_list = Gio::ListStore<MConfiguracion>::create();

    auto contenedor_data = database.sqlite3->get_result();

    for (size_t i = 0; i < contenedor_data["valor"].size(); i++)
    {
        m_list->append(MConfiguracion::create(
            std::stoi(contenedor_data["id"][i]),
            contenedor_data["descripcion"][i],
            contenedor_data["valor"][i]
        ));
    }

    return m_list;
}

void Configuracion::update_conf(const Glib::RefPtr<MConfiguracion> &conf)
{

}
