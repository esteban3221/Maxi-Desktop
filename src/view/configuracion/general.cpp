#include "view/configuracion/general.hpp"

VGeneral::VGeneral(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                            m_builder(refBuilder)
{
}

VGeneral::~VGeneral()
{
}
