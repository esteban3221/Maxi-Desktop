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
            i["storedInCashbox"].get<int>(),
            i["storedInPayout"].get<int>(),
            i["Inmovilidad_Min"].get<int>(),
            i["Inmovilidad"].get<int>(),
            i["Inmovilidad_Max"].get<int>(),
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
        std::cerr << "Error parseando denominacion individual: " << e.what();
    }
    return {};
}