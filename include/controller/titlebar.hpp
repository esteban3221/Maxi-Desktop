#pragma once
#include "view/titlebar.hpp"


class TitleBar : public VTitlebar
{
private:
    sigc::connection m_conn;

    void init_list_ip(void);
    bool poll_ip(void);
    void on_ety_servidor_activate(void);
public:
    TitleBar(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~TitleBar();
};