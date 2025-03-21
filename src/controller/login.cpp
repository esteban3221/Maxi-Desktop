#include "controller/login.hpp"
#include "login.hpp"

Login::Login(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VLogin(cobject, refBuilder) 
{
    v_btn_acceder->signal_clicked().connect(sigc::mem_fun(*this, &Login::on_btn_login_clicked));
    ((Gtk::Entry *)v_ety_passwd)->signal_activate().connect(sigc::mem_fun(*this, &Login::on_btn_login_clicked));
}

Login::~Login()
{
}

void Login::on_btn_login_clicked()
{
    auto header = cpr::Header{{"Content-Type", "application/x-www-form-urlencoded"}};
    auto payload = cpr::Payload{{"grant_type", "password"},{"password", v_ety_passwd->get_text()}};
    auto r = cpr::Post(cpr::Url{Global::System::URL, "sesion/login"}, header, payload);

    if (r.status_code == cpr::status::HTTP_OK)
    {
        auto j = nlohmann::json::parse(r.text);
        
        Global::User::Current = j["usuario"].get<std::string>();
        Global::System::token = j["token"].get<std::string>();
        Global::Widget::v_main_title->set_text(Global::User::Current);
        Global::Widget::v_main_stack->set_visible_child("menu");
    }
    else
    {
        Global::Widget::v_revealer->set_reveal_child(true);
        Global::Widget::v_revealer_title->set_text("Usuario o contraseña incorrecta");
    }
}