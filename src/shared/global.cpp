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

        Gtk::Revealer *v_revealer = nullptr;
        Gtk::Label *v_revealer_title = nullptr;
        Gtk::ProgressBar *v_progress_bar = nullptr;

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
