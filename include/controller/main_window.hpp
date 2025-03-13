#pragma once
#include <gtkmm.h>
#include <iostream>
#include "global.hpp"
#include "controller/titlebar.hpp"
#include "controller/login.hpp"

class MainWindow : public Gtk::Window
{
private:


public:
    MainWindow(/* args */);
    ~MainWindow();
};