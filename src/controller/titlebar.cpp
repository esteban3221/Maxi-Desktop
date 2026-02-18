#include "controller/titlebar.hpp"
#include "titlebar.hpp"

TitleBar::TitleBar(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VTitlebar(cobject, refBuilder)
{
    init_list_ip();
    async.dispatcher.connect(sigc::mem_fun(async, &Global::Async::on_dispatcher_emit));
    v_ety_servidor->signal_activate().connect(sigc::mem_fun(*this, &TitleBar::on_ety_servidor_activate));
    // v_btn_regresar->signal_clicked().connect(sigc::mem_fun(*this, &TitleBar::on_btn_regresar_clicked));
    v_list_ip->signal_row_activated().connect(sigc::mem_fun(*this, &TitleBar::on_list_ip_row_activated));
    check_compatibilidad();

    ws.connect(Global::System::WS + "/ws/heartbeat", 
        sigc::mem_fun(*this, &TitleBar::on_ws_open),
        sigc::mem_fun(*this, &TitleBar::on_ws_message),
        sigc::mem_fun(*this, &TitleBar::on_ws_error),
        sigc::mem_fun(*this, &TitleBar::on_ws_close),
        true
);
}

TitleBar::~TitleBar()
{
    ws.close();
}

void TitleBar::on_btn_regresar_clicked(void)
{
    //Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate();
}

void TitleBar::init_list_ip(void)
{
    auto list_ip = std::make_unique<ListIp>();
    auto list_store = list_ip->get_all_ip();

    for (size_t i = 0; i < list_store->get_n_items(); i++)
    {
        v_list_ip->prepend(*Gtk::manage(new ListItem(list_store->get_item(i))));

        if(list_store->get_item(i)->m_selected)
        {
            Global::System::IP = list_store->get_item(i)->m_ip;
            Global::System::URL = "http://" + Global::System::IP + ":44333/";
            Global::System::WS = "ws://" + Global::System::IP + ":44333";
        }
    }
}

void TitleBar::on_ety_servidor_activate(void)
{
    auto bd = std::make_unique<ListIp>();
    auto m_list_ip = MListIp::create(-1, v_ety_servidor->get_text());
    m_list_ip->m_id = bd->insert(m_list_ip);
    bd->predeterminar(m_list_ip->m_id);
    Global::System::IP = m_list_ip->m_ip;
    Global::System::URL = "http://" + Global::System::IP + ":44333/";
    Global::System::WS = "ws://" + Global::System::IP + ":44333";

    for (auto &&i : v_list_ip->get_children())
    {
        auto item = dynamic_cast<ListItem *>(i);
        if (item)
            item->img_select->set_visible(false);
    }

    v_list_ip->prepend(*Gtk::manage(new ListItem(m_list_ip)));
    v_ety_servidor->set_text("");
}

void TitleBar::on_list_ip_row_activated(Gtk::ListBoxRow *row)
{
    auto item = dynamic_cast<ListItem *>(row);
    if (!item)
        return;

    auto bd = std::make_unique<ListIp>();

    bd->predeterminar(item->m_id);
    Global::System::IP = item->m_ip;
    Global::System::URL = "http://" + Global::System::IP + ":44333/";
    Global::System::WS = "ws://" + Global::System::IP + ":44333";

    for (auto &&i : v_list_ip->get_children())
    {
        auto item = dynamic_cast<ListItem *>(i);
        item->img_select->set_visible(false);
    }

    item->img_select->set_visible(true);
}

void TitleBar::on_ws_open()
{
    g_debug("Conectado al WebSocket");
}

void TitleBar::check_compatibilidad()
{
    auto json = nlohmann::json::object();
    json["version"] = Maxicajero::Version::getVersion();
    #ifdef __linux__
    json["plataform"] = "linux";
    #elif _WIN32
    json["plataform"] = "windows";
    #elif __ANDROID__
    json["plataform"] = "android";
    #elif __APPLE__
    json["plataform"] = "macos";
    #else
    json["plataform"] = "unknown";
    #endif

    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "api/compatibilidad"}, cpr::Body{json.dump()}, cpr::Header{{"Content-Type", "application/json"}});
    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
    {   
        if (response.status_code == 200)
        {
            auto json = nlohmann::json::parse(response.text);
        
            if (json["status"].get<std::string>() == "compatible")
            {
                Maxicajero::VersionUtils::Version clientVer = Maxicajero::VersionUtils::Version::fromString(Maxicajero::Version::getVersion());
                Maxicajero::VersionUtils::Version serverVer = Maxicajero::VersionUtils::Version::fromString(json["local_server_version"].get<std::string>());

                Glib::signal_idle().connect_once([this, clientVer, serverVer]() {
                    if (!Maxicajero::VersionUtils::CompatibilityChecker::isCompatible(clientVer, serverVer, Maxicajero::VersionUtils::CompatibilityChecker::Policy::BACKWARD))
                    {
                        Global::Widget::reveal_toast("Versión cliente (" + clientVer.toString() + ") incompatible con el servidor (" + serverVer.toString() + ")", (Gtk::MessageType)3, 10000);
                        Global::System::URL.clear();
                        v_menu_status->set_label("Desconectado");
                        v_menu_status->set_css_classes({"destructive-action"});
                        ws.close();
                        return;
                    }

                g_info("Versión compatible con el servidor.");
                v_menu_status->set_label("Conectado");
                v_menu_status->set_css_classes({"suggested-action"});
                Global::Widget::v_revealer->set_reveal_child(false);
                });
            }
            else
            {
                Glib::signal_idle().connect_once([this]() {
                    Global::Widget::reveal_toast("Versión incompatible con el servidor", (Gtk::MessageType)3, 10000);
                    Global::System::URL.clear();
                    v_menu_status->set_label("Desconectado");
                    v_menu_status->set_css_classes({"destructive-action"});
                    ws.close();
                });
            }
        }
    });
}

void TitleBar::on_ws_message(const std::string& msg)
{
    Glib::signal_idle().connect_once([this, msg]() {  
    auto json = nlohmann::json::parse(msg);
    auto mensaje = json["status"].get<std::string>();
    auto code = json["code"].get<int>();

    switch (code)
    {
    case cpr::status::HTTP_TEMPORARY_REDIRECT:
        v_menu_status->set_css_classes({"warning"});
        v_menu_status->set_label(mensaje);
        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->set_property("enabled", false);
        break;
    case cpr::status::HTTP_OK:
        v_menu_status->set_css_classes({"suggested-action"});
        v_menu_status->set_label(mensaje);
        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->set_property("enabled", true);
        break;
    case cpr::status::HTTP_BAD_REQUEST:
        v_menu_status->set_css_classes({"destructive-action"});
        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->set_property("enabled", false);
        Global::Widget::reveal_toast(mensaje, (Gtk::MessageType)3, 5000);
        break;
    case cpr::status::HTTP_CONTINUE:
        v_menu_status->set_css_classes({"plain"});
        v_menu_status->set_label(mensaje);
        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->set_property("enabled", false);
        break;
    case cpr::status::HTTP_INTERNAL_SERVER_ERROR:
        v_menu_status->set_css_classes({"destructive-action"});
        Global::Widget::reveal_toast(mensaje, (Gtk::MessageType)3, 5000);
        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->set_property("enabled", false);
        break;
    
    default:
        break;
    }});
}

void TitleBar::on_ws_error(const std::string& err)
{
    Glib::signal_idle().connect_once([this, err]() 
    {
        v_menu_status->set_label("Desconectado");
        v_menu_status->set_css_classes({"destructive-action"});
        Global::Widget::reveal_toast(err, (Gtk::MessageType)3, 5000);
    });
}

void TitleBar::on_ws_close(int code, const std::string& reason)
{
    std::cout << "Cerrado: " << reason << " (code " << code << ")" << std::endl;
}
