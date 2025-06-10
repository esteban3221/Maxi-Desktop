#include "controller/login.hpp"
#include "login.hpp"

Login::Login(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VLogin(cobject, refBuilder)
{
    v_btn_acceder->signal_clicked().connect(sigc::mem_fun(*this, &Login::on_btn_login_clicked));
    ((Gtk::Entry *)v_ety_passwd)->signal_activate().connect(sigc::mem_fun(*this, &Login::on_btn_login_clicked));
    signal_map().connect(sigc::mem_fun(*this, &Login::on_show_map));
}

Login::~Login()
{
}

void Login::on_btn_login_clicked()
{
    try
    {
        auto header = cpr::Header{{"Content-Type", "application/x-www-form-urlencoded"}};
        auto payload = cpr::Payload{{"grant_type", "password"}, {"password", v_ety_passwd->get_text()}};
        auto r = cpr::Post(cpr::Url{Global::System::URL, "sesion/login"}, header, payload);

        if (r.status_code == cpr::status::HTTP_OK)
        {
            auto j = nlohmann::json::parse(r.text);

            Global::User::Current = j["usuario"].get<std::string>();
            Global::System::token = j["token"].get<std::string>();
            
            Global::Utility::header = cpr::Header{{"Authorization", "Bearer " + Global::System::token}};
            Global::Widget::v_main_title->set_text(Global::User::Current);
            Global::Widget::v_main_stack->set_visible_child("menu");
        }
        else
        {
            Global::Widget::reveal_toast("Usuario o contraseña incorrecta",Gtk::MessageType::ERROR);
        }
    }
    catch (const std::exception &e)
    {
        
        Global::Widget::reveal_toast(e.what(),Gtk::MessageType::ERROR);
    }
}
void Login::on_show_map()
{
    v_ety_passwd->delete_text(0,v_ety_passwd->get_text().size());

    auto db = std::make_unique<Configuracion>();
    auto data = db->get_conf_data(2,2);

    v_lbl_titulo->set_text(data->get_item(0)->m_valor);
}