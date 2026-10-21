#include "level_cash.hpp"

LevelCash::LevelCash() 
{
}

LevelCash::~LevelCash()
{
}

Glib::RefPtr<Gio::ListStore<MLevelCash>> LevelCash::get_level_cash(const nlohmann::json_abi_v3_12_0::json &json)
{
    auto m_list = Gio::ListStore<MLevelCash>::create();

    for (auto &&i : json)
    {
        m_list->append(MLevelCash::create(
            i["value"].get<int>() / 100,
            i["storedInCashbox"].is_null() ? 0 : i["storedInCashbox"].get<int>(),
            i["storedInPayout"].is_null() ? 0 : i["storedInPayout"].get<int>(),
            i.contains("Inmovilidad_Min") && not i["Inmovilidad_Min"].is_null() ? i["Inmovilidad_Min"].get<u_int16_t>() : -1,
            i.contains("Inmovilidad") && not i["Inmovilidad"].is_null() ? i["Inmovilidad"].get<u_int16_t>() : -1,
            i.contains("Inmovilidad_Max") && not i["Inmovilidad_Max"].is_null() ? i["Inmovilidad"].get<u_int16_t>() : -1,
            i.contains("Ingreso") ? i["Ingreso"].get<int>() : 0
        ));
    }

    return m_list;
}

void LevelCash::update_level_cash(const Glib::RefPtr<MLevelCash> &level)
{

}


Glib::RefPtr<MLevelCash> LevelCash::from_json(const nlohmann::json_abi_v3_12_0::json &j)
{
try
    {
        if (j.is_array() && !j.empty())
        {
            const auto& item = j[0]; 

            return MLevelCash::create(
                item.at("Denominacion").get<int>(), 
                item.at("Almacenado").get<int>(),
                item.at("Recyclador").get<int>(),
                item.at("Inmovilidad_Min").get<int>(),
                item.at("Inmovilidad").get<int>(),
                item.at("Inmovilidad_Max").get<int>(),
                item.value("Ingreso", 0) // Usa 0 si "Ingreso" no existe
            );
        }
    }
    catch(const std::exception& e)
    {
        g_critical("Error parseando denominacion individual: %s", e.what());
    }
    return {};
}