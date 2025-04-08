#pragma once
#include "view/configuracion/empresa.hpp"
#include "global.hpp"

class Empresa : public VEmpresa
{
private:
    void on_show_map();

    void on_btn_clicked();
public:
    Empresa(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Empresa();
};