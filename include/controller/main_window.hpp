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
    
    void on_button_clicked();
    void acceleretors(const Glib::RefPtr<Gtk::Application>& app);

    Gtk::AboutDialog about;

public:
    MainWindow(const Glib::RefPtr<Gtk::Application>& app);
    ~MainWindow();
};

#include <cstdlib>
#include <string>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

class MailTo {
public:
    static bool enviar(const std::string& to, 
                      const std::string& subject = "", 
                      const std::string& body = "") {
        
        std::string url = construirURL(to, subject, body);
        
        #ifdef _WIN32
            return abrirEnWindows(url);
        #else
            return abrirEnUnix(url);
        #endif
    }

private:
    static std::string construirURL(const std::string& to, 
                                   const std::string& subject, 
                                   const std::string& body) {
        std::string url = "mailto:" + to;  // No codificar el email aquí
        
        if (!subject.empty() || !body.empty()) {
            url += "?";
        }
        
        if (!subject.empty()) {
            url += "subject=" + codificarParametro(subject);
        }
        
        if (!body.empty()) {
            if (!subject.empty()) {
                url += "&";
            }
            url += "body=" + codificarParametro(body);
        }
        
        return url;
    }
    
    static std::string codificarParametro(const std::string& texto) {
        std::string encoded;
        for (char c : texto) {
            if (c == ' ') {
                encoded += "%20";
            } else if (c == '\n') {
                encoded += "%0A";
            } else if (c == '&') {
                encoded += "%26";
            } else if (c == '=') {
                encoded += "%3D";
            } else if (c == '?') {
                encoded += "%3F";
            } else if (c == '%') {
                encoded += "%25";
            } else {
                encoded += c;
            }
        }
        return encoded;
    }
    
    #ifdef _WIN32
    static bool abrirEnWindows(const std::string& url) {
        std::cout << "URL generada: " << url << std::endl;
        
        // Método 1: Usar ShellExecute (recomendado)
        HINSTANCE result = ShellExecuteA(
            NULL,
            "open",
            url.c_str(),
            NULL,
            NULL,
            SW_SHOWNORMAL
        );
        
        if ((intptr_t)result > 32) {
            return true;
        }
        
        // Método 2: Fallback con comando escapado correctamente
        std::string comando = "rundll32.exe url.dll,FileProtocolHandler \"" + url + "\"";
        std::cout << "Ejecutando comando: " << comando << std::endl;
        
        return system(comando.c_str()) == 0;
    }
    #endif
    
    static bool abrirEnUnix(const std::string& url) {
        std::string comando = "xdg-open \"" + url + "\"";
        std::cout << "Ejecutando: " << comando << std::endl;
        return system(comando.c_str()) == 0;
    }
};