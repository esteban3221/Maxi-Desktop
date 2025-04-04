#pragma once
#include "view/configuracion/general.hpp"

class General : public VGeneral
{
private:
    /* data */
public:
    General(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~General();
};