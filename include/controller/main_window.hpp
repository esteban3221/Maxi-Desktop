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

class MailTo {
public:
    static bool enviar(const std::string& to, 
                      const std::string& subject = "", 
                      const std::string& body = "") {
        
        std::string url = construirURL(to, subject, body);
        std::string comando = construirComando(url);
        
        if (comando.empty()) {
            std::cerr << "Error: No se pudo determinar cómo abrir el mailto" << std::endl;
            return false;
        }
        
        std::cout << "Abriendo: " << url << std::endl;
        
        int resultado = system(comando.c_str());
        
        if (resultado != 0) {
            std::cerr << "Error ejecutando el comando" << std::endl;
            return false;
        }
        
        return true;
    }

private:
    static std::string construirURL(const std::string& to, 
                                   const std::string& subject, 
                                   const std::string& body) {
        std::string url = "mailto:" + codificarURL(to);
        
        if (!subject.empty() || !body.empty()) {
            url += "?";
        }
        
        if (!subject.empty()) {
            url += "subject=" + codificarURL(subject);
        }
        
        if (!body.empty()) {
            if (!subject.empty()) {
                url += "&";
            }
            url += "body=" + codificarURL(body);
        }
        
        return url;
    }
    
    static std::string codificarURL(const std::string& texto) {
        std::string encoded;
        for (char c : texto) {
            if (c == ' ') {
                encoded += "%20";
            } else if (c == '\n') {
                encoded += "%0A";  // Nueva línea en URL
            } else {
                encoded += c;
            }
        }
        return encoded;
    }
    
    static std::string construirComando(const std::string& url) {
        #ifdef __linux__
            return "xdg-open \"" + url + "\"";
        #elif __APPLE__
            return "open \"" + url + "\"";
        #elif _WIN32
            return "start \"" + url + "\"";
        #else
            return "xdg-open \"" + url + "\"";
        #endif
    }
};
