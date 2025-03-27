#include "level_cash.hpp"

LevelCash::LevelCash() 
{
}

LevelCash::~LevelCash()
{
}

Glib::RefPtr<Gio::ListStore<MLevelCash>> LevelCash::get_level_cash(const nlohmann::json_abi_v3_11_3::json &json)
{
    auto m_list = Gio::ListStore<MLevelCash>::create();

    for (auto &&i : json)
    {
        m_list->append(MLevelCash::create(
            i["Denominacion"].get<int>(),
            i["Almamcenado"].get<int>(),
            i["Recyclador"].get<int>(),
            i["Inmovilidad"].get<int>(),
            0
        ));
    }

    return m_list;
}

void LevelCash::update_level_cash(const Glib::RefPtr<MLevelCash> &level)
{

}
