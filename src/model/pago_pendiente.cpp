#include "pago_pendiente.hpp"

PagoPendiente::PagoPendiente(/* args */)
{
}

PagoPendiente::~PagoPendiente()
{
}

Glib::RefPtr<Gio::ListStore<MPagoPendiente>> PagoPendiente::get_log_historial()
{
    auto m_list = Gio::ListStore<MPagoPendiente>::create();


    return m_list;
}

void PagoPendiente::update_log_historial(const Glib::RefPtr<MPagoPendiente> &list)
{

}