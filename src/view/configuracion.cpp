#include "view/configuracion.hpp"

VConfiguracion::VConfiguracion(/* args */)
{

    auto builder = Gtk::Builder::create_from_file("../ui/menu/conf/general.ui");
    auto general = Gtk::Builder::get_widget_derived<General>(builder, "boxConfGeneral");

    append_page(*general,"General");

    builder = Gtk::Builder::create_from_file("../ui/menu/conf/detalle_sistema.ui");
    auto sistema = Gtk::Builder::get_widget_derived<Sistema>(builder, "infSistemas");

    append_page(*sistema,"Detalle");

    for (auto &&i : get_children())
    {
        i->set_margin(10);
    }
    
}

VConfiguracion::~VConfiguracion()
{
}
