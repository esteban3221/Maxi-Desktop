#include "controller/configuracion/sistema.hpp"

Sistema::Sistema(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VSistema(cobject, refBuilder) 
{
    signal_map().connect(sigc::mem_fun(*this, &Sistema::on_show_map));
    v_list_acciones->signal_row_activated().connect(sigc::mem_fun(*this, &Sistema::on_click_list));
}

Sistema::~Sistema()
{
}

void Sistema::on_click_list(Gtk::ListBoxRow *row)
{
    auto index = row->get_index();
    std::optional<cpr::AsyncResponse> future;
    if (index == 0) 
        future = cpr::GetAsync(cpr::Url{Global::System::URL + "configuracion/reiniciar"}, Global::Utility::header);
    else if (index == 1) 
        future = cpr::GetAsync(cpr::Url{Global::System::URL + "configuracion/apagar"}, Global::Utility::header);

    if (future)
        Global::Utility::consume_and_do(future.value(), [this](const cpr::Response &response)
        {
            if (response.status_code == cpr::status::HTTP_OK) 
            {
                Global::Widget::reveal_toast("Iniciando proceso", Gtk::MessageType::INFO, 3000);
                Global::Widget::v_main_stack->set_visible_child("login");
                Global::Widget::v_main_title->set_text("Maxicajero");
            }
            else 
                Global::Widget::reveal_toast("Error al procesar la solicitud", Gtk::MessageType::ERROR);
        }
    );
    
}

void Sistema::on_show_map()
{
    auto future = cpr::GetAsync(cpr::Url{Global::System::URL + "configuracion/get_informacion_sistema"}, Global::Utility::header);
    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
    {

                if (response.status_code == cpr::status::HTTP_OK) 
                {
                    auto j = nlohmann::json::parse(response.text);

                    auto memory = j["memory"].get<std::string>();
                    auto ram = j["ram"].get<std::string>();
                    auto processor = j["processor"].get<std::string>();
                    auto model = j["model"].get<std::string>();
                    auto hostname = j["hostname"].get<std::string>();

                    v_lbl_sistema[0]->set_text(hostname);
                    v_lbl_sistema[1]->set_text(model);
                    v_lbl_sistema[2]->set_text(processor);
                    v_lbl_sistema[3]->set_text(ram);
                    v_lbl_sistema[4]->set_text(memory);

                    if (j["coin"].contains("error") || j["bill"].contains("error")) 
                    {
                        auto error = j["coin"].contains("error") ? j["coin"]["error"].get<std::string>() : j["bill"]["error"].get<std::string>();
                        auto dialog = Gtk::MessageDialog(*Global::Widget::v_main_window, "Error");
                        dialog.set_secondary_text(error);
                        dialog.set_hide_on_close();
                        dialog.set_visible();
                    } 
                    else 
                    {
                        v_lbl_titulo_coin->set_text(j["coin"]["deviceModel"].get<std::string>());
                        v_lbl_coin[0]->set_text(j["coin"]["deviceID"].get<std::string>());

                        v_lbl_coin[1]->set_text(j["coin"]["deviceModel"].get<std::string>());
                        v_lbl_coin[2]->set_text(std::to_string(j["coin"]["isOpen"].get<bool>()));
                        v_lbl_coin[3]->set_text(std::to_string(j["coin"]["sspProtocolVersion"].get<int>()));
                        v_lbl_coin[4]->set_text(j["coin"]["deviceError"].get<std::string>());
                        v_lbl_coin[5]->set_text(j["coin"]["firmware"].get<std::string>());
                        v_lbl_coin[6]->set_text(j["coin"]["dataset"].get<std::string>());
                        v_lbl_coin[7]->set_text(j["coin"]["primaryHopperSerialNumber"].get<std::string>());
                        v_lbl_coin[8]->set_text(j["coin"]["coinFeederSerialNumber"].get<std::string>());
                        v_lbl_coin[9]->set_text(j["coin"]["primaryHopperBuildRevision"].get<std::string>());
                        v_lbl_coin[10]->set_text(j["coin"]["coinFeederBuildRevision"].get<std::string>());
                        v_lbl_coin[11]->set_text(j["coin"]["realTimeClock"].get<std::string>());
                        
                        v_lbl_titulo_bill->set_text(j["bill"]["deviceModel"].get<std::string>());
                        v_lbl_bill[0]->set_text(j["bill"]["deviceID"].get<std::string>());

                        v_lbl_bill[1]->set_text(j["bill"]["deviceModel"].get<std::string>());
                        v_lbl_bill[2]->set_text(std::to_string(j["bill"]["isOpen"].get<bool>()));
                        v_lbl_bill[3]->set_text(std::to_string(j["bill"]["sspProtocolVersion"].get<int>()));
                        v_lbl_bill[4]->set_text(j["bill"]["deviceError"].get<std::string>());
                        v_lbl_bill[5]->set_text(j["bill"]["firmware"].get<std::string>());
                        v_lbl_bill[6]->set_text(j["bill"]["dataset"].get<std::string>());
                        v_lbl_bill[7]->set_text(j["bill"]["validatorSerialNumber"].get<std::string>());
                        v_lbl_bill[8]->set_text(j["bill"]["payoutModuleSerialNumber"].get<std::string>());
                        v_lbl_bill[9]->set_text(j["bill"]["validatorRevision"].get<std::string>());
                        v_lbl_bill[10]->set_text(j["bill"]["payoutModuleRevision"].get<std::string>());
                        v_lbl_bill[11]->set_text(j["bill"]["realTimeClock"].get<std::string>());
                    }
                    
                }
            });
}