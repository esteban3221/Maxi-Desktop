#pragma once
#include <gtkmm.h>
#include "global.hpp"

#include "controller/configuracion/general.hpp"
#include "controller/configuracion/sistema.hpp"

class VConfiguracion : public Gtk::Notebook
{
private:
    /* data */
public:
    VConfiguracion(/* args */);
    ~VConfiguracion();
};