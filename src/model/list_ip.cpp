#include "model/list_ip.hpp"

ListIp::ListIp()
{
}

ListIp::~ListIp()
{
}

Glib::RefPtr<Gio::ListStore<MListIp>> ListIp::get_all_ip()
{
    auto &database = Database::getInstance();
    auto contenedor_data = database.sqlite3->command("select * from ip");
    auto m_list = Gio::ListStore<MListIp>::create();

    if(contenedor_data->empty())
        return m_list;
    
    for (size_t i = 0; i < contenedor_data->at("id").size(); i++)
    {
        m_list->append(MListIp::create(std::stoi(contenedor_data->at("id")[i]), contenedor_data->at("ip")[i]));
    }
    
    return m_list;
}

size_t ListIp::insert(const Glib::RefPtr<MListIp> &item)
{
    auto &database = Database::getInstance();
    database.sqlite3->command("insert into ip (ip) values (?)", item->m_ip.c_str());

    auto contenedor_data = database.sqlite3->command("select id from ip order by id desc limit 1");
    return std::stoull(contenedor_data->at("id")[0]);
}

void ListIp::delete_ip(const Glib::RefPtr<MListIp> &item)
{
    auto &database = Database::getInstance();
    database.sqlite3->command("delete from ip where id = ?", item->m_id);
}
