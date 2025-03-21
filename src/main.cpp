#include "controller/main_window.hpp"

int main(int argc, char *argv[])
{   
    auto app = Gtk::Application::create("org.desktop.maxicajero");
    return app->make_window_and_run<MainWindow>(argc, argv, app);   
}
