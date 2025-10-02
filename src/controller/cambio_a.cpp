#include "controller/cambio_a.hpp"

CambioA::CambioA(/* args */)
{
    set_orientation(Gtk::Orientation::VERTICAL);
    property_homogeneous() = true;
    v_ety_concepto.set_margin_top(10);
    v_ety_concepto.set_placeholder_text("Concepto");
    v_ety_concepto.set_css_classes({"title-1"});
    v_ety_concepto.set_margin_start(100);
    v_ety_concepto.set_margin_end(100);
    v_ety_concepto.set_size_request(-1,100);
    v_ety_concepto.property_valign() = Gtk::Align::CENTER;
    Global::Utility::set_multiline_text(v_ety_concepto);
    v_lbl_info.set_label("Monto ingresado");
    v_lbl_info.set_css_classes({"title-3","dim-label"});
    v_lbl_ingreso.set_label("$0.00");
    v_lbl_ingreso.set_css_classes({"title-1","bold-label"});
    v_btn_empezar.set_label("Empezar");
    v_btn_empezar.property_halign() = Gtk::Align::CENTER;
    v_btn_empezar.property_valign() = Gtk::Align::CENTER;
    v_btn_empezar.set_css_classes({"suggested-action"});
    append(v_ety_concepto);
    append(v_lbl_info);
    append(v_lbl_ingreso);
    append(v_btn_empezar);
    set_margin_top(20);
    set_margin_bottom(20);
    set_margin_start(20);
    set_margin_end(20);
    set_spacing(10);

    v_btn_empezar.signal_clicked().connect(sigc::mem_fun(*this, &CambioA::on_btn_empezar_clicked));
}

CambioA::~CambioA()
{
}

void CambioA::on_btn_empezar_clicked()
{
    set_sensitive(false);
    nlohmann::json json = {
        {"concepto", v_ety_concepto.get_text()}};

    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/inicia_cambio"}, Global::Utility::header, cpr::Body{json.dump()});

    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
                                    {
                if (response.status_code == 200) 
                {
                    auto j = nlohmann::json::parse(response.text);

                    auto log = std::make_unique<Log>();
                    auto m_log = log->get_log(j["ticket"]);
                    auto ticket = m_log->get_item(0);
                    auto faltante = j["Cambio_faltante"].get<int>();

                    Global::Widget::reveal_toast(Glib::ustring::compose("<span weight=\"bold\"> %5 </span>\n\n"
                                                                        "Total: \t\t$%1\n"
                                                                        "Cambio: \t$%2\n"
                                                                        "Ingreso: \t$%3\n"
                                                                        "Faltante: \t$%4\n", 
                                                                        ticket->m_total, 
                                                                        ticket->m_cambio, 
                                                                        ticket->m_ingreso, 
                                                                        faltante,
                                                                        "Cambio Automatico"), Gtk::MessageType::OTHER);

                    Global::System::imprime_ticket(ticket, faltante);

                    if (faltante > 0) 
                    {
                        // v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window,"Cambio Faltante",false,Gtk::MessageType::INFO, Gtk::ButtonsType::NONE));
                        // v_dialog->set_secondary_text(Glib::ustring::format("Se requiere un cambio de " , faltante));
                        // v_dialog->set_visible();
                        Global::Widget::reveal_toast(Glib::ustring::compose("<span weight=\"bold\">Cambio Automatico</span>\n\n"
                                                                        "Se requiere un cambio de $%1", faltante), Gtk::MessageType::WARNING);
                    }
                } else 
                {
                    // v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Error"));
                    // v_dialog->set_secondary_text(response.text);
                    // v_dialog->set_visible();
                    Global::Widget::reveal_toast(Glib::ustring::compose("Error: %1", response.text), Gtk::MessageType(3));
                }
                Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate();
                set_sensitive(true); });
}