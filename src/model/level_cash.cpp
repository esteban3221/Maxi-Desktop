#include "level_cash.hpp"

LevelCash::LevelCash(const std::string &tipo) : TIPO(tipo)
{
}

LevelCash::~LevelCash()
{
}

Glib::RefPtr<Gio::ListStore<MLevelCash>> LevelCash::get_level_cash()
{

    auto m_list_log = Gio::ListStore<MLevelCash>::create();


    return m_list_log;
}

void LevelCash::update_level_cash(const Glib::RefPtr<MLevelCash> &level)
{

}
