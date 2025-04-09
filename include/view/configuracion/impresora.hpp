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

    struct
    {
        Gtk::ListBox *v_lbox_activa;
        Gtk::Switch *v_switch_activa;

        Gtk::ListBox *v_lbox_vizualizador;
        Gtk::CheckButton *v_check_vizualizador[6]{nullptr};

        Gtk::ListBox *v_lbox_test;
    }local,remoto;

    Gtk::ListBox *v_list_box_print;
    Gtk::Notebook *v_ntbook_propiedades;
    Gtk::TextView *v_text_view;
public:
    VImpresora(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VImpresora();
};
