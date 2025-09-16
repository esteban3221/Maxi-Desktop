#pragma once
#include <gtkmm.h>
#include "global.hpp"
#include "controller/configuracion/impresora.hpp"

class CambioA : public Gtk::Box
{
private:
    Gtk::Entry v_ety_concepto;
    Gtk::Label v_lbl_info;
    Gtk::Label v_lbl_ingreso;
    Gtk::Button v_btn_empezar;

    void on_btn_empezar_clicked();
public:
    CambioA(/* args */);
    ~CambioA();
};