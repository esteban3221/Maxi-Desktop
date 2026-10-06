#include "global.hpp"
// #ifdef ERROR
// #undef ERROR
// #endif

namespace Global
{
    namespace Widget
    {
        Gtk::Stack *v_main_stack = nullptr;
        Gtk::Window *v_main_window = nullptr;
        Gtk::Label *v_main_title = nullptr;
        Gtk::Button *v_button_conatiner = nullptr;

        Gtk::Revealer *v_revealer = nullptr;
        Gtk::Label *v_revealer_title = nullptr;
        Gtk::ProgressBar *v_progress_bar = nullptr;
        Glib::RefPtr<Gio::SimpleActionGroup> m_refActionGroup = nullptr;
        ;

        void reveal_toast(const Glib::ustring &title, Gtk::MessageType type, int duration)
        {
            switch (type)
            {
            case Gtk::MessageType::INFO:
                v_button_conatiner->set_css_classes({"osd"});
                break;
            case Gtk::MessageType::WARNING:
                v_button_conatiner->set_css_classes({"warning"});
                break;
            case Gtk::MessageType::QUESTION:
                v_button_conatiner->set_css_classes({"suggested-action"});
                break;
            case (Gtk::MessageType)3:
                v_button_conatiner->set_css_classes({"destructive-action"});
                break;
            case Gtk::MessageType::OTHER:
                v_button_conatiner->set_css_classes({"button"});
                break;

            default:
                break;
            }
            v_revealer_title->set_markup(title);
            v_revealer_title->set_css_classes({"title", "h4"});
            v_revealer->set_reveal_child();
        }

        namespace Impresora
        {
            bool state_vizualizacion[6]{false}, is_activo{false};
        } // namespace Impresora

    } // namespace Widget

    namespace Utility
    {
#ifdef _WIN32
        std::string WStrToUTF8(const wchar_t *wstr)
        {
            if (!wstr) return ""; 

            int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
            if (size_needed <= 1) return ""; // Protege si la cadena está vacía o falla

            std::string utf8_str(size_needed - 1, 0);
            WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &utf8_str[0], size_needed, NULL, NULL);
            return utf8_str;
        }
#endif

        cpr::Header header{{"Authorization", "Bearer " + Global::System::token}};
        void consume_and_do(cpr::AsyncResponse &async, const std::function<void(const cpr::Response &)> &callback)
        {
            System::is_in_process.store(true);
            auto lookup = Widget::m_refActionGroup->lookup_action("cerrarsesion");
            std::thread([async = std::move(async), callback, lookup]() mutable
            {
                try 
                {
                    while (async.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) 
                    {
                        Glib::signal_idle().connect_once([lookup]() 
                        {
                            if(lookup) 
                            {
                                lookup->set_property("enabled", false);
                                Widget::v_main_window->set_deletable(false);
                            }
                            Global::Widget::v_progress_bar->pulse();
                        });
                    }
        
                    auto response = async.get();
        
                    Glib::signal_idle().connect_once([response, callback, lookup]() 
                    {
                        if(lookup) 
                        {
                            lookup->set_property("enabled", true);
                            Widget::v_main_window->set_deletable(true);
                        }
                        Global::Widget::v_progress_bar->set_fraction(1.0);
                        callback(response);
                    });
                    System::is_in_process.store(false);
                } 
                catch (const std::exception& e) 
                {
                    g_warning(e.what());
                    System::is_in_process.store(false);
                    Glib::signal_idle().connect_once([lookup, e]() 
                    {
                        if(lookup) 
                        {
                            lookup->set_property("enabled", true);
                            Widget::v_main_window->set_deletable(true);
                            Global::Widget::reveal_toast("Error de conexión : " + std::string(e.what()), Gtk::MessageType::WARNING);
                        }
                        Global::Widget::v_progress_bar->set_fraction(1.0);
                    });
                    Widget::m_refActionGroup->lookup_action("cerrarsesion")->set_property("enabled", true);
                } 
            }).detach();
        }

        void set_multiline_text(Gtk::Entry &entry)
        {
            entry.property_truncate_multiline() = false;
            entry.property_primary_icon_name() = "insert-text-symbolic";
            entry.signal_icon_press().connect([&entry](Gtk::Entry::IconPosition position)
                                              {
                if (position == Gtk::Entry::IconPosition::PRIMARY) {
                    auto text = entry.get_text();
                    auto cursor_pos = entry.get_position();
                    text.insert(cursor_pos, "\n");
                    entry.set_text(text);
                    entry.set_position(cursor_pos + 1);
                } });
        }

    } // namespace Utility

    namespace System
    {
        Glib::ustring IP = "";
        std::string WS = "";
        std::string URL{"http://" + IP + ":44333/"};
        std::string token = "";
        std::atomic_bool is_in_process = false;
    } // namespace System

    namespace User
    {
        std::string Current = "";
        int id = -1;
    } // namespace User

} // namespace Helper
