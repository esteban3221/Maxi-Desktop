#pragma once
#include "view/configuracion/usuarios.hpp"

class CUsuarios : public VUsuarios
{
private:
    /* data */
public:
    CUsuarios(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~CUsuarios();
};