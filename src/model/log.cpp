#include "log.hpp"

Log::Log(/* args */)
{
    
}

Log::~Log()
{
}

Glib::RefPtr<Gio::ListStore<MLog>> Log::get_log(const nlohmann::json_abi_v3_12_0::json &json)
{
    auto m_list = Gio::ListStore<MLog>::create();

    for (auto &&i : json)
    {
        m_list->append(MLog::create
        (
            i["id"].get<int>(),
            i["usuario"].get<std::string>(),
            i["tipo"].get<std::string>(),
            i["descripcion"].get<std::string>(),
            i["ingreso"].get<int>(),
            i["cambio"].get<int>(),
            i["total"].get<int>(),
            i["estatus"].get<std::string>(),
            Glib::DateTime::create_from_iso8601(i["fecha"].get<std::string>())
        ));
    }
    return m_list;
}

// size_t Log::insert_log(const Glib::RefPtr<MLog> &list)
// {
//     return std::stoull("0");
// }

// void Log::update_log(const Glib::RefPtr<MLog> &list)
// {
// }
