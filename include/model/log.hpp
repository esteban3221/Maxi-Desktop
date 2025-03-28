#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <glibmm.h>
#include <giomm.h>

#include "global.hpp"

class MLog : public Glib::Object
{
public:
    size_t m_id;
    Glib::ustring m_user;
    Glib::ustring m_tipo;
    int m_ingreso;
    int m_cambio;
    int m_total;
    Glib::ustring m_estatus;
    Glib::DateTime m_fecha;

    static Glib::RefPtr<MLog> create(size_t id,
                                     const Glib::ustring & user,
                                      const Glib::ustring &tipo,
                                      int ingreso,
                                      int cambio,
                                      int total,
                                      const Glib::ustring &estatus,
                                      Glib::DateTime fecha)
    {
        return Glib::make_refptr_for_instance<MLog>(new MLog(id, user, tipo, ingreso, cambio, total, estatus, fecha));
    }

protected:
    MLog(size_t id,
            const Glib::ustring &user,
          const Glib::ustring &tipo,
          int ingreso,
          int cambio,
          int total,
          const Glib::ustring &estatus,
          const Glib::DateTime &fecha)
        : m_id(id),
          m_user(user),
          m_tipo(tipo),
          m_ingreso(ingreso),
          m_cambio(cambio),
          m_total(total),
          m_estatus(estatus),
          m_fecha(fecha)
    {
    }
};

class Log
{
private:
public:
    Log(/* args */);
    ~Log();
    Glib::RefPtr<Gio::ListStore<MLog>> get_log(const nlohmann::json_abi_v3_11_3::json &json);

    // void imprime_log();
};
