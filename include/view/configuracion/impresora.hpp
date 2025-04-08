#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VImpresora : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;

protected:

    class VListPrinters : public Gtk::ListBoxRow
    {
    public:
        Gtk::Label *v_titulo;
        Gtk::Image *v_image_check;

        VListPrinters(const std::string &titulo,const std::string &subtitulo);
    private:
        Gtk::Label *v_subtitulo;
        Gtk::Image *v_image_printer;
        Gtk::Box *v_box;
    };

    Gtk::ListBox *v_list_box_print;
public:
    VImpresora(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VImpresora();
};
