#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <glibmm.h>
#include <giomm.h>

#include "global.hpp"

class MLevelCash : public Glib::Object
{
public:
    uint16_t m_denominacion;
    uint16_t m_cant_alm;
    uint16_t m_cant_recy;
    uint16_t m_nivel_inmo_min;
    uint16_t m_nivel_inmo;
    uint16_t m_nivel_inmo_max;
    uint16_t m_ingreso;

    static Glib::RefPtr<MLevelCash> create(uint16_t denominacion, uint16_t cant_alm, uint16_t cant_recy, uint16_t nivel_inmo_min, uint16_t nivel_inmo, uint16_t nivel_inmo_max, uint16_t ingreso)
    {
        return Glib::make_refptr_for_instance<MLevelCash>(new MLevelCash(denominacion, cant_alm, cant_recy, nivel_inmo_min, nivel_inmo, nivel_inmo_max, ingreso));
    }

protected:
    MLevelCash(uint16_t denominacion, uint16_t cant_alm, uint16_t cant_recy, uint16_t nivel_inmo_min, uint16_t nivel_inmo, uint16_t nivel_inmo_max, uint16_t ingreso)
        : m_denominacion(denominacion), m_cant_alm(cant_alm), m_cant_recy(cant_recy), m_nivel_inmo_min(nivel_inmo_min), m_nivel_inmo(nivel_inmo), m_nivel_inmo_max(nivel_inmo_max), m_ingreso(ingreso)
    {
    }
}; // ModelColumns

class LevelCash
{
private:

public:
    LevelCash();
    ~LevelCash();

    // std::vector<std::shared_ptr<Model::LevelCash_t>> get_log();
    Glib::RefPtr<Gio::ListStore<MLevelCash>> get_level_cash(const nlohmann::json_abi_v3_12_0::json &json);
    void update_level_cash(const Glib::RefPtr<MLevelCash> &level);
};