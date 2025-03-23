#pragma once
#include "view/titlebar.hpp"
#include "global.hpp"

class TitleBar : public VTitlebar
{
private:
    void init_list_ip(void);
    bool poll_ip(void);
    void on_ety_servidor_activate(void);

    Global::Async async;
public:
    TitleBar(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~TitleBar();
};