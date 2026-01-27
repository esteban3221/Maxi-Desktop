#pragma once
#include "view/titlebar.hpp"
#include "ws_client.hpp"
#include "global.hpp"
#include "configuracion/version.hpp"
#include "shared/version_comparator.hpp"

class TitleBar : public VTitlebar
{
private:
    void init_list_ip(void);
    void on_ety_servidor_activate(void);
    void on_btn_regresar_clicked(void);

    Global::Async async;
    IXWSClient ws;

public:
    TitleBar(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~TitleBar();
};

