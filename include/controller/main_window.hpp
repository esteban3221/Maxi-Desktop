#pragma once
#include <gtkmm.h>
#include <iostream>
#include "global.hpp"
#include "controller/titlebar.hpp"
#include "controller/login.hpp"

class MainWindow : public Gtk::Window
{
private:
    Gtk::Overlay v_overlay;
    Gtk::Button v_button;

public:
    MainWindow(/* args */);
    ~MainWindow();
};