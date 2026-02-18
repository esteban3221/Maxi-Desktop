#pragma once
#include "view/venta.hpp"
#include "model/log.hpp"
#include "controller/configuracion/impresora.hpp"
#include "ws_client.hpp"

class Venta : public VVenta
{
private:
    void on_btn_enter_clicked();
    void on_map_show();

    bool is_view_ingreso;
    void on_btn_cancelar_clicked();

    // WebSocket client para comunicación en tiempo real
    IXWSClient ws;
    void enviar_datos_venta();
    void manejar_respuesta_servidor(const std::string& respuesta);

public:
    Venta(bool is_view_ingreso = false);
    ~Venta();
};