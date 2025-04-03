#include "view/configuracion/sistema.hpp"

VSistema::VSistema(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                            m_builder(refBuilder)
{
}

VSistema::~VSistema()
{
}
