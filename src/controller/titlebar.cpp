#include "controller/titlebar.hpp"
#include "titlebar.hpp"

TitleBar::TitleBar(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VTitlebar(cobject, refBuilder)
{
    init_list_ip();
    async.dispatcher.connect(sigc::mem_fun(async, &Global::Async::on_dispatcher_emit));
    v_ety_servidor->signal_activate().connect(sigc::mem_fun(*this, &TitleBar::on_ety_servidor_activate));
    v_btn_regresar->signal_clicked().connect(sigc::mem_fun(*this, &TitleBar::on_btn_regresar_clicked));
}

TitleBar::~TitleBar()
{
    ws.close();
}

void TitleBar::on_btn_regresar_clicked(void)
{
     Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate();
}

void TitleBar::init_list_ip(void)
{
    auto list_ip = std::make_unique<ListIp>();
    auto list_store = list_ip->get_all_ip();

    ///@todo: recordar la ultima ip usada y seleccionarla al iniciar la app
    for (size_t i = 0; i < list_store->get_n_items(); i++)
    {
        v_list_ip->prepend(*Gtk::manage(new ListItem(list_store->get_item(i))));
        Global::System::IP = list_store->get_item(i)->m_ip;

        Global::System::URL = "http://" + Global::System::IP + ":44333/";
        Global::System::WS = "ws://" + Global::System::IP + ":44333";
    }

    ws.connect(Global::System::WS + "/ws/heartbeat", [this]() 
    {
        g_debug("Conectado al WebSocket");
        
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

        ws.send(json.dump());
    },
    [this](const std::string& msg) 
    {
        auto json = nlohmann::json::parse(msg);
        if (json["status"].get<std::string>() == "compatible")
        {
            Maxicajero::VersionUtils::Version clientVer = Maxicajero::VersionUtils::Version::fromString(Maxicajero::Version::getVersion());
            Maxicajero::VersionUtils::Version serverVer = Maxicajero::VersionUtils::Version::fromString(json["local_server_version"].get<std::string>());

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
        }
        else
        {
            Global::Widget::reveal_toast("Versión incompatible con el servidor", (Gtk::MessageType)3, 10000);
            Global::System::URL.clear();
            v_menu_status->set_label("Desconectado");
            v_menu_status->set_css_classes({"destructive-action"});
            ws.close();
        }
    },

    [this](const std::string& err) {
        v_menu_status->set_label("Desconectado");
        v_menu_status->set_css_classes({"destructive-action"});
        Global::Widget::reveal_toast(err, (Gtk::MessageType)3, 5000);
    },
    [](int code, const std::string& reason) {
        std::cout << "Cerrado: " << reason << " (code " << code << ")" << std::endl;
    }
);
}

void TitleBar::on_ety_servidor_activate(void)
{
    auto bd = std::make_unique<ListIp>();
    auto m_list_ip = MListIp::create(-1, v_ety_servidor->get_text());
    m_list_ip->m_id = bd->insert(m_list_ip);
    Global::System::IP = m_list_ip->m_ip;
    Global::System::URL = "http://" + Global::System::IP + ":44333/";

    v_list_ip->prepend(*Gtk::manage(new ListItem(m_list_ip)));
    v_ety_servidor->set_text("");
}
