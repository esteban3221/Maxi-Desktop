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
