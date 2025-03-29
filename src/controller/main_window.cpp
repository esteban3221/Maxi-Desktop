#include "controller/main_window.hpp"
#include "main_window.hpp"

MainWindow::MainWindow(const Glib::RefPtr<Gtk::Application>& app)
{
    set_default_size(1280, 720);
    //set_margin(15);

    Global::Widget::v_main_window = this;
    Global::Widget::v_main_stack = Gtk::manage(new Gtk::Stack());
    Global::Widget::v_main_title = Gtk::manage(new Gtk::Label());

    Global::Widget::v_revealer = Gtk::manage(new Gtk::Revealer());
    Global::Widget::v_revealer_title = Gtk::manage(new Gtk::Label());

    Global::Widget::v_revealer->set_halign(Gtk::Align::CENTER);
    Global::Widget::v_revealer->set_valign(Gtk::Align::START);
    Global::Widget::v_revealer->set_margin_top(30);

    Global::Widget::v_revealer_title->set_text("Configuración");
    Gtk::Box *v_box = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL, 30));
    v_button.set_icon_name("window-close-symbolic");
    v_button.set_css_classes({"circular"});

    v_box->append(*Global::Widget::v_revealer_title);
    v_box->append(v_button);

    v_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_clicked));

    Global::Widget::v_revealer->set_transition_type(Gtk::RevealerTransitionType::SWING_DOWN);
    Global::Widget::v_revealer->set_child(*v_box);

    auto builder = Gtk::Builder::create_from_file("../ui/titlebar.ui");
    auto titlebar = Gtk::Builder::get_widget_derived<TitleBar>(builder, "header");

    builder = Gtk::Builder::create_from_file("../ui/login.ui");
    auto login = Gtk::Builder::get_widget_derived<Login>(builder, "box_login");

    builder = Gtk::Builder::create_from_file("../ui/menu.ui");
    auto menu = Gtk::Builder::get_widget_derived<VMenu>(builder, "box_menu");

    Global::Widget::v_main_stack->set_transition_type(Gtk::StackTransitionType::ROTATE_LEFT_RIGHT);
    Global::Widget::v_main_stack->set_transition_duration(250);

    Global::Widget::v_main_stack->add(*login, "login");
    Global::Widget::v_main_stack->add(*menu, "menu");

    set_titlebar(*titlebar);
    v_overlay.set_child(*Global::Widget::v_main_stack);
    v_overlay.add_overlay(*Global::Widget::v_revealer);
    
    acceleretors(app);

    set_child(v_overlay);
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_button_clicked()
{
    Global::Widget::v_revealer->set_reveal_child(false);
}
void MainWindow::acceleretors(const Glib::RefPtr<Gtk::Application>& app)
{
    m_refActionGroup = Gio::SimpleActionGroup::create();

    m_refActionGroup->add_action("quit", [this]() { this->close(); });
    m_refActionGroup->add_action("about",  [this]() { 
        about.set_transient_for(*this);
        about.set_program_name("Maxicajero Desktop");
        about.set_artists({"Maxicajero"});
        about.set_authors({"Maxicajero","Esteban Noé"});
        about.set_version("1.0.0");
        about.set_license_type(Gtk::License::BSD_3);
        about.set_copyright("Maxicajero");
        about.set_comments("Aplicación de escritorio para el manejo de cajeros automáticos");
        about.set_logo_icon_name("utilities-terminal");
        about.set_title("Acerca de MaxiCajero");
        about.set_modal(true);
        about.set_hide_on_close(true);
        about.set_visible(true); });

    insert_action_group("app", m_refActionGroup);

    app->set_accel_for_action("app.quit", "<Primary>q");
    app->set_accel_for_action("app.about", "<Primary>i");
}