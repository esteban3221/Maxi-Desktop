#pragma once
#include "view/configuracion/empresa.hpp"

class Empresa : public VEmpresa
{
private:
    /* data */
public:
    Empresa(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Empresa();
};