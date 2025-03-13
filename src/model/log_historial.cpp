#include "log_historial.hpp"

LogHistorial::LogHistorial(/* args */)
{
}

LogHistorial::~LogHistorial()
{
}

Glib::RefPtr<Gio::ListStore<MLogHistorial>> LogHistorial::get_log_historial()
{
    auto m_list = Gio::ListStore<MLogHistorial>::create();

    return m_list;
}

/// @brief Inserta a BD (Fecha se debe de pasar normal, esta funcion ya hace la conversion a iso8601)
/// @param list 
void LogHistorial::insert_log_historial(const Glib::RefPtr<MLogHistorial> &list)
{

}