#include "controller/configuracion/terminal.hpp"


Terminal::Terminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VTerminal(cobject, refBuilder)
{
    v_btn_add_terminal->signal_clicked().connect(sigc::mem_fun(*this, &Terminal::on_button_add_clicked));

    ///@todo solo de prueba, luego se eliminara
    for (size_t i = 0; i < 200; i++)
    {
        auto card = Gtk::manage(new VCardMP("Terminal " + std::to_string(i + 1), "computer"));
        card->set_alias("MP Terminal " + std::to_string(i + 1));
        card->set_tipo("Contacto + QR");
        card->set_descripcion(i%2 == 0 ? "Terminal principal para pagos rápidos" : "");
        card->set_icon_name("terminal-pos-symbolic");
        v_flow_terminals->append(*card);

        card->signal_clicked().connect([this, i]() {
            v_rvl->set_reveal_child(true);
            v_btn_add_terminal->set_visible(false);
        });
    }
}

Terminal::~Terminal()
{
}

void Terminal::on_button_add_clicked()
{

    auto form_terminal_builder = Gtk::Builder::create_from_string(View::Conf::form_terminal_ui);
    auto form_terminal = form_terminal_builder->get_widget_derived<VTerminal::VFormTerminal>(form_terminal_builder,"box_form");
    form_terminal->v_btn_close_rvl->signal_clicked().connect(sigc::mem_fun(*this, &Terminal::on_button_close_rvl_clicked));

    v_rvl->set_child(*form_terminal);
    v_rvl->set_reveal_child(true);
    v_btn_add_terminal->set_visible(false);
}

void Terminal::on_button_close_rvl_clicked()
{
    v_rvl->set_reveal_child(false);
    v_btn_add_terminal->set_visible(true);
}