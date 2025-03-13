#include "log.hpp"

Log::Log(/* args */)
{
    Glib::init();
}

Log::~Log()
{
}

Glib::RefPtr<Gio::ListStore<MLog>> Log::get_log()
{
    auto m_list = Gio::ListStore<MLog>::create();

    return m_list;
}

size_t Log::insert_log(const Glib::RefPtr<MLog> &list)
{
    
    return std::stoull("0");
}

void Log::update_log(const Glib::RefPtr<MLog> &list)
{
    
}
