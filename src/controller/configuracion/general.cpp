#include "controller/configuracion/general.hpp"

General::General(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VGeneral(cobject, refBuilder) 
{
    signal_map().connect(sigc::mem_fun(*this, &General::on_show_mapping));
    v_ety_mensaje_inicio->signal_changed().connect(sigc::mem_fun(*this, &General::on_ety_change_mensaje_inicio));
    v_btn_reinicia_val->signal_clicked().connect(sigc::mem_fun(*this, &General::on_btn_reinicia_val_clicked));
}

General::~General()
{
}

void General::on_show_mapping()
{
    auto db = std::make_unique<Configuracion>();
    auto data = db->get_conf_data(2,2);

    v_ety_mensaje_inicio->set_text(data->get_item(0)->m_valor);
}

void General::on_ety_change_mensaje_inicio()
{
    auto db = std::make_unique<Configuracion>();
    db->update_conf(MConfiguracion::create(2,"Mensaje Inicio", v_ety_mensaje_inicio->get_text()));
}

void General::on_btn_reinicia_val_clicked()
{
    nlohmann::json json = {
        {"rol", 9},
        {"bill", {
            {"command", "ResetDevice"},
            {"args", ""}
        }},
        {"coin", {
            {"command", "ResetDevice"},
            {"args", ""}
        }}
    };
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "configuracion/custom_command"}, Global::Utility::header , cpr::Body{json.dump()});
    Global::Utility::consume_and_do(future,[this](cpr::Response response)
    {
        if (response.status_code == 200) 
        {
            Global::Widget::v_revealer->set_reveal_child();
            Global::Widget::v_revealer_title->set_text("Exito");
        }

        std::cout << response.text << std::endl;
    });
}