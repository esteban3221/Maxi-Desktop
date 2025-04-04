#pragma once
#include <gtkmm.h>
#include "global.hpp"

#include "controller/configuracion/general.hpp"
#include "controller/configuracion/sistema.hpp"
#include "controller/configuracion/impresora.hpp"
#include "controller/configuracion/empresa.hpp"
#include "controller/configuracion/usuarios.hpp"

class VConfiguracion : public Gtk::Notebook
{
private:
    /* data */
public:
    VConfiguracion(/* args */);
    ~VConfiguracion();
};