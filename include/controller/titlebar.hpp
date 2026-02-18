#pragma once
#include "view/titlebar.hpp"
#include "global.hpp"
#include "configuracion/version.hpp"
#include "shared/version_comparator.hpp"
#include "ws_client.hpp"

class TitleBar : public VTitlebar
{
private:
    void init_list_ip(void);
    void init_ws(void);
    void on_ety_servidor_activate(void);
    void on_btn_regresar_clicked(void);
    void on_list_ip_row_activated(Gtk::ListBoxRow *row);

    void check_compatibilidad();

    Global::Async async;

    IXWSClient ws;
    void on_ws_open();
    void on_ws_message(const std::string& msg);
    void on_ws_error(const std::string& error);
    void on_ws_close(int code, const std::string& reason);

public:
    TitleBar(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~TitleBar();
};

