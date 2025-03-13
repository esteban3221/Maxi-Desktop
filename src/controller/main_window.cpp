#include "controller/main_window.hpp"

MainWindow::MainWindow(/* args */)
{
    set_default_size(1280, 720);
    set_margin(15);

    Global::Widget::v_main_window = this;
    Global::Widget::v_main_stack = Gtk::manage(new Gtk::Stack());
    Global::Widget::v_main_title = Gtk::manage(new Gtk::Label());

    Global::Widget::v_revealer = Gtk::manage(new Gtk::Revealer());
    Global::Widget::v_revealer_title = Gtk::manage(new Gtk::Label());

    Global::Widget::v_revealer->set_halign(Gtk::Align::CENTER);
    Global::Widget::v_revealer->set_valign(Gtk::Align::START);
    Global::Widget::v_revealer->set_margin_top(30);

    Global::Widget::v_revealer_title->set_text("Configuración");
    Gtk::Box *v_box = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL,30));
    v_button.set_icon_name("window-close-symbolic");
    v_button.set_css_classes({"circular"});

    v_box->append(*Global::Widget::v_revealer_title);
    v_box->append(v_button);

    Global::Widget::v_revealer->set_transition_type(Gtk::RevealerTransitionType::SWING_DOWN);
    Global::Widget::v_revealer->set_child(*v_box);
    Global::Widget::v_revealer->set_reveal_child(true);

    auto builder = Gtk::Builder::create_from_file("../ui/titlebar.ui");
    auto titlebar = Gtk::Builder::get_widget_derived<TitleBar>(builder, "header");

    builder = Gtk::Builder::create_from_file("../ui/login.ui");
    auto login = Gtk::Builder::get_widget_derived<Login>(builder, "box_login");

    Global::Widget::v_main_stack->add(*login);

    set_titlebar(*titlebar);
    v_overlay.set_child(*Global::Widget::v_main_stack);
    v_overlay.add_overlay(*Global::Widget::v_revealer);
    set_child(v_overlay);
}

MainWindow::~MainWindow()
{
}