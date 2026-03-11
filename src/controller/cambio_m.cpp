#include "cambio_m.hpp"

CambioM::CambioM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : VPagoM(cobject, refBuilder)
{

    // View
    v_btn_cobrar->set_label("Iniciar");
    v_btn_aceptar.set_label("Dar Cambio");
    v_btn_cancelar.set_label("Cancelar");
    v_btn_aceptar.set_css_classes({"suggested-action"});
    v_btn_cancelar.set_css_classes({"destructive-action"});
    v_box_botones.set_halign(Gtk::Align::CENTER);
    v_box_botones.append(v_btn_aceptar);
    v_box_botones.append(v_btn_cancelar);
    append(v_box_botones);

    //Controller
    this->signal_map().connect(sigc::mem_fun(*this, &CambioM::on_show_map));
    v_btn_cobrar->signal_clicked().connect(sigc::mem_fun(*this, &CambioM::on_btn_cobrar_clicked));
    v_btn_aceptar.signal_clicked().connect(sigc::mem_fun(*this, &CambioM::on_btn_aceptar_clicked));
    v_btn_cancelar.signal_clicked().connect(sigc::mem_fun(*this, &CambioM::on_btn_cancelar_clicked));
}

void CambioM::on_show_map()
{
    v_btn_aceptar.set_sensitive(false);
    v_btn_cancelar.set_sensitive(false);
    v_btn_cobrar->set_sensitive(true);
    ingreso = total = 0;
}

CambioM::~CambioM()
{
}

void CambioM::activa_spinners(Gtk::SpinButton *spinner[], int pos, int tam)
{
    if (pos == -1)
        for (int i = 0; i < tam; i++)
            spinner[i]->set_sensitive(true);
    else
        for (int i = 0; i < tam; i++)
            if (i <= pos)
                spinner[i]->set_sensitive(true);
            else
                spinner[i]->set_sensitive(false);
}

void CambioM::on_spin_value_changed()
{
    total = 0;

    v_btn_aceptar.set_label(Glib::ustring::compose("Cambio $ %1", total));
}

void CambioM::on_btn_cobrar_clicked()
{
    v_btn_cobrar->set_sensitive(false);
    v_btn_cobrar->set_label("Espere...");
    nlohmann::json json = {{"concepto", v_ety_concepto->get_text()}};
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/inicia_cambio_manual"}, Global::Utility::header, cpr::Body{json.dump()});

    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
                                    {
                                        if (response.status_code == 200)
                                        {
                                            auto j = nlohmann::json::parse(response.text);                            
                                            ingreso = j["ingresado"].get<int>();
                                            int pos_coin = j["pos_coin"].get<int>();
                                            int pos_bill = j["pos_bill"].get<int>();

                                            // activa_spinners(v_spin_bill, pos_bill, 6);
                                            // activa_spinners(v_spin_coin, pos_coin, 4);

                                            v_btn_aceptar.set_sensitive(true);
                                            v_btn_cancelar.set_sensitive(true);
                                            
                                        } });
}

void CambioM::on_btn_aceptar_clicked()
{

}

void CambioM::on_btn_cancelar_clicked()
{
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/cancelar_cambio_manual"},
                                 Global::Utility::header);

    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
                                    {
                                        if (response.status_code == 200)
                                                Global::Widget::reveal_toast("Cambio Manual Cancelado");
                                            
                                        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate(); });
}