#pragma once
#include <iostream>
#include <atomic>
#include <queue>
#include <gtkmm.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */
    
// por conveniencia algunas cosas estan en ingles
namespace Global
{
    namespace Widget
    {
        // manejador global de vistas
        extern Gtk::Stack *v_main_stack;
        extern Gtk::Window *v_main_window;
        extern Gtk::Label *v_main_title;

        extern Gtk::Revealer *v_revealer;
        extern Gtk::Label *v_revealer_title;
        extern Gtk::ProgressBar *v_progress_bar;

        namespace Impresora
        {
            extern bool state_vizualizacion[6] ,is_activo;
        } // namespace Impresora
        
    } // namespace Widget

    namespace Utility
    {
        extern void consume_and_do(cpr::AsyncResponse &async , const std::function<void(const cpr::Response &)> &callback);
        #ifdef __WIN32__
        #include <locale>
        #include <codecvt>
        extern std::string WStrToUTF8(const wchar_t* wstr);
        #endif
        
    } // namespace Utility

    namespace EValidador
    {
        struct Balance
        {
            std::atomic<uint32_t> ingreso = 0;
            std::atomic<uint32_t> ingreso_parcial = 0;
            std::atomic<uint32_t> total = 0;
            std::atomic<int32_t> cambio = 0;
        } extern balance;
    } // namespace EstadoValidador

    namespace System
    {
        extern Glib::ustring IP;
        extern std::string URL;
        extern std::string token;
    } // namespace System

    class Async
    {
    private:
        std::queue<std::function<void()>> dispatch_queue;
        std::mutex dispatch_queue_mutex;

    public:
        Async(/* args */) {};
        ~Async() {};

        Glib::Dispatcher dispatcher;
        void on_dispatcher_emit();
        void dispatch_to_gui(std::function<void()> func);
    };

    inline void Async::on_dispatcher_emit()
    {
        std::function<void()> func;
        {
            std::lock_guard<std::mutex> lock(dispatch_queue_mutex);
            if (!dispatch_queue.empty())
            {
                func = dispatch_queue.front();
                dispatch_queue.pop();
            }
        }
        if (func)
        {
            func();
        }
    }

    inline void Async::dispatch_to_gui(std::function<void()> func)
    {
        {
            std::lock_guard<std::mutex> lock(dispatch_queue_mutex);
            dispatch_queue.push(func);
        }
        dispatcher.emit();
    }

    namespace User
    {
        extern std::string Current;
        extern int id;
    } // namespace User

} // namespace Helper
