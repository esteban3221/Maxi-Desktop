#include "controller/configuracion/terminal.hpp"


Terminal::Terminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VTerminal(cobject, refBuilder)
{
    v_btn_add_terminal->signal_clicked().connect(sigc::mem_fun(*this, &Terminal::on_button_add_clicked));
    v_btn_close_rvl->signal_clicked().connect(sigc::mem_fun(*this, &Terminal::on_button_close_rvl_clicked));

    ///@todo solo de prueba, luego se eliminara
    for (size_t i = 0; i < 20; i++)
    {
        auto card = Gtk::manage(new VCardMP("Terminal " + std::to_string(i + 1), "computer"));
        card->set_alias("MP Terminal " + std::to_string(i + 1));
        card->set_tipo("Contacto + QR");
        card->set_descripcion("Terminal principal para pagos rápidos");
        card->set_icon_name("terminal-pos-symbolic");   // o el que tengas
        v_flow_terminals->append(*card);

        card->signal_clicked().connect([this, i]() {
            v_rvl->set_reveal_child(true);
            v_lbl_titulo_rvl->set_text("Detalles de la Terminal " + std::to_string(i + 1));
            v_btn_add_terminal->set_visible(false);
        });
    }
}

Terminal::~Terminal()
{
}

void Terminal::on_button_add_clicked()
{
    v_rvl->set_reveal_child(true);
    v_lbl_titulo_rvl->set_text("Agregar Nueva Terminal");
    v_btn_add_terminal->set_visible(false);
}

void Terminal::on_button_close_rvl_clicked()
{
    v_rvl->set_reveal_child(false);
    v_btn_add_terminal->set_visible(true);
}