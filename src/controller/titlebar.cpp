#include "controller/titlebar.hpp"
#include "titlebar.hpp"

TitleBar::TitleBar(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VTitlebar(cobject, refBuilder)
{
    init_list_ip();
    async.dispatcher.connect(sigc::mem_fun(async, &Global::Async::on_dispatcher_emit));
    v_ety_servidor->signal_activate().connect(sigc::mem_fun(*this, &TitleBar::on_ety_servidor_activate));
}

TitleBar::~TitleBar()
{
}

void TitleBar::init_list_ip(void)
{
    auto list_ip = std::make_unique<ListIp>();
    auto list_store = list_ip->get_all_ip();

    for (size_t i = 0; i < list_store->get_n_items(); i++)
    {
        v_list_ip->prepend(*Gtk::manage(new ListItem(list_store->get_item(i))));
        Global::System::IP = list_store->get_item(i)->m_ip;

        Global::System::URL = "http://" + Global::System::IP + ":44333/";
    }

    std::thread([this]() { while (poll_ip())  std::this_thread::sleep_for(std::chrono::seconds(5)); })
        .detach();
}

bool TitleBar::poll_ip(void)
{
    auto r = cpr::Post(cpr::Url{Global::System::URL, "test_coneccion"});

    if (r.status_code == cpr::status::HTTP_OK)
    {
        async.dispatch_to_gui([this](){
            v_menu_status->set_label("Conectado");
            v_menu_status->set_css_classes({"suggested-action"});
        });
        
    }
    else
    {
        async.dispatch_to_gui([this](){
            v_menu_status->set_label("Desconectado");
            v_menu_status->set_css_classes({"destructive-action"});
        });
    }

    return true;
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
