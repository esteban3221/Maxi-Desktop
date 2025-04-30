#pragma once
#include "view/configuracion/general.hpp"
#include "model/configuracion.hpp"

class General : public VGeneral
{
private:
    void on_show_mapping();
    void on_ety_change_mensaje_inicio();
    void on_btn_reinicia_val_clicked();
public:
    General(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~General();
};