#pragma once
#include "view/configuracion/impresora.hpp"
#include "model/configuracion.hpp"
#include "model/log.hpp"
#ifdef __linux__
#include "cups/cups.h"
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <winspool.h>
#endif
#include "global.hpp"

class Impresora : public VImpresora
{
private:
    void init_impresoras_linux();

    struct PrinterInfo {
        std::string name;  // Nombre de la impresora
        std::string uri;   // URI o ubicación (para Linux) / Puerto (para Windows)
        bool is_default;   // Si es la impresora predeterminada
    };

    std::vector<PrinterInfo> listarImpresoras();
    void init_impresoras_windows();

    void init_local();
    void init_remoto();
    void on_list_box_row_selected(Gtk::ListBoxRow *);
    void on_activalist_activate(Gtk::ListBoxRow *row);
    void on_vizaliza_list_activate(Gtk::ListBoxRow *row);
    void on_vizaliza_list_activate_remoto(Gtk::ListBoxRow *row);
    void on_switch_tab_changed(Gtk::Widget *, guint);    
    void on_list_remoto_guardar(Gtk::ListBoxRow *);
    void test_text_impresion(int id);

    Glib::RefPtr<Gtk::TextBuffer> text_buffer ;
    
public:
    Impresora(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Impresora();
    
};

namespace Global
{
    namespace System
    {
        extern std::string imprime_ticket(Glib::RefPtr<MLog> log, int faltante = 0);
    } // namespace System
}