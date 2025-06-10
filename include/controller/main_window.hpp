#pragma once
#include <gtkmm.h>
#include <iostream>
#include "global.hpp"

#include "view/menu.hpp"

#include "controller/titlebar.hpp"
#include "controller/login.hpp"

class MainWindow : public Gtk::Window
{
private:
    Gtk::Overlay v_overlay;
    Gtk::Button v_button;
    

    Glib::RefPtr<Gio::SimpleActionGroup> m_refActionGroup;
    Glib::RefPtr<Gio::SimpleAction> m_refActionRain;

    void on_button_clicked();
    void acceleretors(const Glib::RefPtr<Gtk::Application>& app);

    Gtk::AboutDialog about;

public:
    MainWindow(const Glib::RefPtr<Gtk::Application>& app);
    ~MainWindow();
};