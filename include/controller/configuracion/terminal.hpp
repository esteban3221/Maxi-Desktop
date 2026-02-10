#pragma once
#include <gtkmm.h>
#include <memory>

#include "view/configuracion/terminal.hpp"
#include "view/configuracion/c_card_mp.hpp"

class Terminal : public VTerminal
{
private:
    Glib::ustring id_terminal_seleccionada;
    Glib::ustring modo_operacion_seleccionado;

    void on_show_map();

    void on_button_add_clicked();
    void on_card_mp_clicked();
    void on_button_close_rvl_clicked();

    //Detalles de terminal
    VDetailsTerminal *details_terminal{nullptr};

    //formulario de agregar terminal
    VFormTerminal *form_terminal{nullptr};

    void on_add_terminal();
    void on_btn_verifica_token_clicked();
    void on_list_terminales_row_activated(Gtk::ListBoxRow *row);
    void on_btn_agregar_clicked();

    void cambia_modo_operacion(const Glib::ustring &id, const Glib::ustring &modo_operacion);

public:
    Terminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Terminal();
};