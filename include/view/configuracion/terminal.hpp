#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VTerminal : public Gtk::Box
{
    private:
    Glib::RefPtr<Gtk::Builder> m_builder;

    protected:
    Gtk::Button *v_btn_add_terminal{nullptr};
    Gtk::FlowBox *v_flow_terminals{nullptr};
    Gtk::Revealer *v_rvl{nullptr};

    //Formulario de agregar terminal
    class VFormTerminal : public Gtk::Box
    {
    private:
        Glib::RefPtr<Gtk::Builder> m_builder;

    protected:
        Gtk::Entry *v_ety_token{nullptr};
        Gtk::Entry *v_ety_alias{nullptr};
        Gtk::Entry *v_ety_descripcion{nullptr};
        Gtk::Button *v_btn_verifica_token{nullptr};
        Gtk::ListBox *v_list_terminales{nullptr};
        Gtk::Label *v_lbl_total_terminales{nullptr};
        Gtk::Button *v_btn_agregar{nullptr};

        //Modelo - Componente de la lista de terminales
        class Row : public Gtk::ListBoxRow
        {
        public:
            Gtk::Label *v_titulo;
            Row(const std::string &titulo,const std::string &subtitulo);
            ~Row();
        private:
            //Modo
            Gtk::Label *v_subtitulo;
            Gtk::Image *v_image_pos;
            Gtk::Box *v_box;
        }; // Row

    public:
        VFormTerminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
        Gtk::Button *v_btn_close_rvl{nullptr};
        ~VFormTerminal();
    }; // VFormTerminal

    public:
    VTerminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VTerminal();
};

namespace View
{
    namespace Conf
    {
        extern const char *terminal_ui;
        extern const char *form_terminal_ui;
    } // namespace Conf
} // namespace View