#pragma once
#include "view/configuracion/general.hpp"
#include "model/configuracion.hpp"
#include "controller/configuracion/impresora.hpp"

class General : public VGeneral
{
private:
    void on_show_mapping();
    void on_ety_change_mensaje_inicio();
    void on_btn_reinicia_val_clicked();
    void on_btn_retirada();

    void on_btn_actualiza_pos_clicked();
    void on_file_dialog_finish(const Glib::RefPtr<Gio::AsyncResult>& result,const Glib::RefPtr<Gtk::FileDialog>& dialog);
public:
    General(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~General();
};