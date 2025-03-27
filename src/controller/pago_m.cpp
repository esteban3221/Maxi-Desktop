#include "controller/pago_m.hpp"
#include "pago_m.hpp"

PagoM::PagoM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VPagoM(cobject, refBuilder) 
{
    for (auto &&i : v_spin_coin)
        i->signal_value_changed().connect(sigc::mem_fun(*this, &PagoM::on_spin_value_changed));
    for (auto &&i : v_spin_bill)
        i->signal_value_changed().connect(sigc::mem_fun(*this, &PagoM::on_spin_value_changed));

    v_btn_cobrar->signal_clicked().connect(sigc::mem_fun(*this, &PagoM::on_btn_cobrar_clicked));
    
}

PagoM::~PagoM()
{
}


void PagoM::on_spin_value_changed()
{
    size_t total = 0;
    for (size_t i = 0; i < 4; i++)
        total += coins.at(i) * v_spin_coin[i]->get_value_as_int();
    
    for (size_t i = 0; i < 6; i++)
        total += bills.at(i) * v_spin_bill[i]->get_value_as_int();
    

    v_btn_cobrar->set_label(Glib::ustring::compose("Cobrar $ %1", total));
}

void PagoM::on_btn_cobrar_clicked()
{
    size_t total = 0;
    for (size_t i = 0; i < 4; i++)
        total += coins.at(i) * v_spin_coin[i]->get_value_as_int();
    
    for (size_t i = 0; i < 6; i++)
        total += bills.at(i) * v_spin_bill[i]->get_value_as_int();
    
    if (total > 0)
    {
        std::thread([this]() {
            try {
                auto json = nlohmann::json{
                    {"bill", nlohmann::json::array({
                        v_spin_bill[0]->get_value_as_int(),
                        v_spin_bill[1]->get_value_as_int(),
                        v_spin_bill[2]->get_value_as_int(),
                        v_spin_bill[3]->get_value_as_int(),
                        v_spin_bill[4]->get_value_as_int(),
                        v_spin_bill[5]->get_value_as_int()
                    })},
                    {"coin", nlohmann::json::array({
                        v_spin_coin[0]->get_value_as_int(),
                        v_spin_coin[1]->get_value_as_int(),
                        v_spin_coin[2]->get_value_as_int(),
                        v_spin_coin[3]->get_value_as_int()
                    })}
                };

                auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/inicia_pago_manual"}, cpr::Body{json.dump()});
    
                while (future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
                    Glib::signal_idle().connect_once([this]() {
                        Global::Widget::v_progress_bar->pulse();
                    });
                }
                auto cpy = future.share();  
    
                Glib::signal_idle().connect_once([this, cpy]() {
                    Global::Widget::v_progress_bar->set_fraction(1.0);
    
                    auto response = cpy.get();
                    
                    if (response.status_code == cpr::status::HTTP_CONFLICT)
                    {
                            v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Error"));
                            v_dialog->set_secondary_text(response.text);
                            v_dialog->set_visible();
                        
                    }
                    else if (response.status_code == cpr::status::HTTP_OK) {
    
                        Global::Widget::v_revealer->set_reveal_child(true);
                        Global::Widget::v_revealer_title->set_text("Pago Manual Realizado");
                    } else {
                        v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Error"));
                        v_dialog->set_secondary_text(response.text);
                        v_dialog->set_visible();
                    }
    
                    set_sensitive(true);
                });
            } catch (const std::exception& e) {
                Glib::signal_idle().connect_once([this, e]() {
                    v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Error"));
                    v_dialog->set_secondary_text("Excepción: No se encuentra el servidor");
                    v_dialog->set_visible();
                    set_sensitive(true);
                });
            }
        }).detach();
    }
    else
    {
        v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Error"));
        v_dialog->set_secondary_text("El monto a vender debe ser mayor a 0");
        v_dialog->set_visible();
    }
    
    
}