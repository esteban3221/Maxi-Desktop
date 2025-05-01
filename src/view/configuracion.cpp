#include "view/configuracion.hpp"

VConfiguracion::VConfiguracion(/* args */)
{

    #ifdef _WIN32
    auto builder = Gtk::Builder::create_from_string(View::Conf::general_ui);
    #else
    auto builder = Gtk::Builder::create_from_file("../ui/menu/conf/general.ui");
    #endif
    auto general = Gtk::Builder::get_widget_derived<General>(builder, "boxConfGeneral");

    append_page(*general,"General");

    #ifdef _WIN32
    builder = Gtk::Builder::create_from_string(View::Conf::sistema_ui);
    #else
    builder = Gtk::Builder::create_from_file("../ui/menu/conf/detalle_sistema.ui");
    #endif
    auto sistema = Gtk::Builder::get_widget_derived<Sistema>(builder, "infSistemas");

    append_page(*sistema,"Detalle");

    #ifdef _WIN32
    builder = Gtk::Builder::create_from_string(View::Conf::empresa_ui);
    #else
    builder = Gtk::Builder::create_from_file("../ui/menu/conf/datos_empresa.ui");
    #endif
    auto empresa = Gtk::Builder::get_widget_derived<Empresa>(builder, "boxDatosEmpresa");

    append_page(*empresa,"Empresa");

    #ifdef _WIN32
    builder = Gtk::Builder::create_from_string(View::Conf::impresora_ui);
    #else
    builder = Gtk::Builder::create_from_file("../ui/menu/conf/impresora.ui");
    #endif
    auto impresora = Gtk::Builder::get_widget_derived<Impresora>(builder, "box_config_impresora");

    append_page(*impresora,"Impresora");

    #ifdef _WIN32
    builder = Gtk::Builder::create_from_string(View::Conf::usuario_ui);
    #else
    builder = Gtk::Builder::create_from_file("../ui/menu/conf/usuarios.ui");
    #endif
    auto usuarios = Gtk::Builder::get_widget_derived<CUsuarios>(builder, "box_usuarios");

    append_page(*usuarios,"Usuarios");

    set_show_border(false);
}

VConfiguracion::~VConfiguracion()
{
}
