#include "controller/main_window.hpp"

MainWindow::MainWindow(/* args */)
{
    set_default_size(720, 480);
    set_margin(15);

    Global::Widget::v_main_window = this;
    Global::Widget::v_main_stack = Gtk::manage(new Gtk::Stack());

    auto builder = Gtk::Builder::create_from_file("../ui/titlebar.ui");
    auto titlebar = Gtk::Builder::get_widget_derived<TitleBar>(builder, "header");

    builder = Gtk::Builder::create_from_file("../ui/login.ui");
    auto login = Gtk::Builder::get_widget_derived<Login>(builder, "box_login");

    Global::Widget::v_main_stack->add(*login);

    set_titlebar(*titlebar);
    set_child(*Global::Widget::v_main_stack);
}

MainWindow::~MainWindow()
{
}