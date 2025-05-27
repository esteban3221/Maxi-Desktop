#include "view/configuracion.hpp"

VConfiguracion::VConfiguracion(/* args */)
{

    auto builder = Gtk::Builder::create_from_string(View::Conf::general_ui);
    auto general = Gtk::Builder::get_widget_derived<General>(builder, "boxConfGeneral");

    append_page(*general,"General");

    builder = Gtk::Builder::create_from_string(View::Conf::sistema_ui);
    auto sistema = Gtk::Builder::get_widget_derived<Sistema>(builder, "infSistemas");

    append_page(*sistema,"Detalle");

    builder = Gtk::Builder::create_from_string(View::Conf::empresa_ui);
    auto empresa = Gtk::Builder::get_widget_derived<Empresa>(builder, "boxDatosEmpresa");

    append_page(*empresa,"Empresa");

    builder = Gtk::Builder::create_from_string(View::Conf::impresora_ui);
    auto impresora = Gtk::Builder::get_widget_derived<Impresora>(builder, "box_config_impresora");

    append_page(*impresora,"Impresora");

    builder = Gtk::Builder::create_from_string(View::Conf::usuario_ui);
    auto usuarios = Gtk::Builder::get_widget_derived<CUsuarios>(builder, "box_usuarios");

    append_page(*usuarios,"Usuarios");

    set_show_border(false);
}

VConfiguracion::~VConfiguracion()
{
}
