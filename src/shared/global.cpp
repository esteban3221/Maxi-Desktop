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
        Glib::RefPtr<Gio::SimpleActionGroup> m_refActionGroup;

        void reveal_toast(const Glib::ustring &title, Gtk::MessageType type, int duration )
        {
            switch (type)
            {
            case Gtk::MessageType::INFO:
                v_button_conatiner->set_css_classes({"pill", "osd"});
            break;
            case Gtk::MessageType::WARNING:
                v_button_conatiner->set_css_classes({"pill", "warning"});
            break;
            case Gtk::MessageType::QUESTION:
                v_button_conatiner->set_css_classes({"pill", "suggested-action"});
            break;
            case (Gtk::MessageType)3:
                v_button_conatiner->set_css_classes({"pill", "destructive-action"});
            break;
            case Gtk::MessageType::OTHER:
                v_button_conatiner->set_css_classes({"button"});
            break;
            
            default:
                break;
            }
            v_revealer_title->set_markup(title);
            v_revealer->set_reveal_child();
        }

        namespace Impresora
        {
            bool state_vizualizacion[6]{false}, is_activo{false};
        } // namespace Impresora

    } // namespace Widget

    namespace Utility
    {
        #ifdef __WIN32__
        std::string WStrToUTF8(const wchar_t* wstr)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            std::string utf8_str = converter.to_bytes(wstr);
            return utf8_str;
        }
        #endif
        cpr::Header header{{"Authorization", "Bearer " + Global::System::token}};
        void consume_and_do(cpr::AsyncResponse &async, const std::function<void(const cpr::Response &)> &callback)
        {
            std::thread([async = std::move(async), callback]() mutable {
                try {
                    while (async.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
                        Glib::signal_idle().connect_once([]() {
                            Global::Widget::v_progress_bar->pulse();
                        });
                    }
        
                    auto response = async.get();
        
                    Glib::signal_idle().connect_once([response, callback]() {
                        Global::Widget::v_progress_bar->set_fraction(1.0);
                        callback(response);
                    });
                } 
                catch (const std::exception& e) 
                {
                    std::cerr << "Error: " << e.what() << std::endl;
                } 
            }).detach();
        }
        
        void set_multiline_text(Gtk::Entry &entry)
        {
            entry.property_truncate_multiline() = false;
            entry.property_primary_icon_name() = "insert-text-symbolic";
            entry.signal_icon_press().connect([&entry](Gtk::Entry::IconPosition position) {
                if (position == Gtk::Entry::IconPosition::PRIMARY) {
                    auto text = entry.get_text();
                    auto cursor_pos = entry.get_position();
                    text.insert(cursor_pos, "\n");
                    entry.set_text(text);
                    entry.set_position(cursor_pos + 1);
                }
            });
        }
    
    } // namespace Utility

    namespace System
    {
        Glib::ustring IP = "";
        std::string URL{"http://" + IP + ":44333/"};
        std::string token = "";
    } // namespace System

    namespace User
    {
        std::string Current = "";
        int id = -1;
    } // namespace User

} // namespace Helper
