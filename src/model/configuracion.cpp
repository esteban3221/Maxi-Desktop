#include "model/configuracion.hpp"

Configuracion::Configuracion(/* args */)
{
}

Configuracion::~Configuracion()
{
}

Glib::RefPtr<Gio::ListStore<MConfiguracion>> Configuracion::get_conf_data(int inicio, int fin)
{
    auto m_list = Gio::ListStore<MConfiguracion>::create();


    return m_list;
}

void Configuracion::update_conf(const Glib::RefPtr<MConfiguracion> &conf)
{

}
