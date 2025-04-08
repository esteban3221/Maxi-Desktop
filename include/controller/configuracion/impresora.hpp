#pragma once
#include "view/configuracion/impresora.hpp"
#include "model/configuracion.hpp"
#include "cups/cups.h"

class Impresora : public VImpresora
{
private:
    void init_impresoras_linux();
    void on_list_box_row_selected(Gtk::ListBoxRow *);

    struct visualizer
    {
        bool activa_impresora = false;
        bool muestra_agradecimiento = false;
        bool muestra_fecha = false;
        bool muestra_direccion = false;
        bool muestra_rfc = false;
        bool muestra_telefono = false;
        bool muestra_vendedor = false;
        bool muestra_contacto = false;
    };
    
    
    
public:
    Impresora(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Impresora();
};