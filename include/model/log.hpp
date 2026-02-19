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
  Glib::ustring m_descripcion;
  Glib::ustring m_ingreso;
  Glib::ustring m_cambio;
  int m_total;
  Glib::ustring m_estatus;
  Glib::DateTime m_fecha;

  static Glib::RefPtr<MLog> create(size_t id,
                                   const Glib::ustring &user,
                                   const Glib::ustring &tipo,
                                   const Glib::ustring &descripcion,
                                   const Glib::ustring &ingreso,
                                   const Glib::ustring &cambio,
                                   int total,
                                   const Glib::ustring &estatus,
                                   Glib::DateTime fecha)
  {
    return Glib::make_refptr_for_instance<MLog>(new MLog(id, user, tipo, descripcion, ingreso, cambio, total, estatus, fecha));
  }

protected:
  MLog(size_t id,
       const Glib::ustring &user,
       const Glib::ustring &tipo,
       const Glib::ustring &descripcion,
       const Glib::ustring &ingreso,
       const Glib::ustring &cambio,
       int total,
       const Glib::ustring &estatus,
       const Glib::DateTime &fecha)
      : m_id(id),
        m_user(user),
        m_tipo(tipo),
        m_descripcion(descripcion),
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
  Glib::RefPtr<Gio::ListStore<MLog>> get_log(const nlohmann::json_abi_v3_12_0::json &json);

  // void imprime_log();
};

class DetalleMovimiento : public Glib::Object
{
public:
  int m_id_log;
  std::string m_tipo_movimiento;
  int m_denominacion;
  int m_cantidad;
  Glib::DateTime m_creado_en;

  static Glib::RefPtr<DetalleMovimiento> create(int id_log, const std::string &tipo,
                                                int denom, int cant,
                                                const Glib::DateTime &fecha)
  {
    return Glib::make_refptr_for_instance<DetalleMovimiento>(
        new DetalleMovimiento(id_log, tipo, denom, cant, fecha));
  }

private:
  DetalleMovimiento(int id_log, const std::string &tipo, int denom, int cant,
                    const Glib::DateTime &fecha)
      : m_id_log(id_log), m_tipo_movimiento(tipo),
        m_denominacion(denom), m_cantidad(cant), m_creado_en(fecha) {}
};