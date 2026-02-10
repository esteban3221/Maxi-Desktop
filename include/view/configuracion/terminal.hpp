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

    public:
        Gtk::Entry *v_ety_token{nullptr};
        Gtk::Entry *v_ety_alias{nullptr};
        Gtk::Entry *v_ety_descripcion{nullptr};
        Gtk::Button *v_btn_verifica_token{nullptr};
        Gtk::DropDown *v_drop_tipo{nullptr};
        Gtk::ListBox *v_list_terminales{nullptr};
        Gtk::Label *v_lbl_total_terminales{nullptr};
        Gtk::Button *v_btn_agregar{nullptr};

        //Modelo - Componente de la lista de terminales
        class Row : public Gtk::ListBoxRow
        {
        public:
            Gtk::Label *v_titulo;
            //Modo
            Gtk::Label *v_subtitulo;
            Row(const std::string &titulo,const std::string &subtitulo);
            ~Row();
        private:
            
            Gtk::Image *v_image_pos;
            Gtk::Box *v_box;
        }; // Row

    public:
        VFormTerminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
        Gtk::Button *v_btn_close_rvl{nullptr};
        ~VFormTerminal();
    }; // VFormTerminal

    class VDetailsTerminal : public Gtk::Box
    {
    private:
        Glib::RefPtr<Gtk::Builder> m_builder;
    public:
        Gtk::Button *v_btn_close_rvl{nullptr};
        Gtk::DropDown *v_drop_tipo{nullptr};
        Gtk::DropDown *v_drop_modo{nullptr};
        Gtk::Entry *v_ety_alias{nullptr};
        Gtk::EditableLabel *v_ety_descripcion{nullptr};
        Gtk::CheckButton *v_chk_predeterminado{nullptr};
        Gtk::Label *v_lbl_fecha_creacion{nullptr};

        Gtk::Button *v_btn_guardar{nullptr};
        Gtk::Button *v_btn_eliminar{nullptr};

        VDetailsTerminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
        ~VDetailsTerminal();
    };

    
    public:
    VTerminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VTerminal();
};

namespace View
{
    namespace Conf
    {
        extern const char *details_terminal_ui;
        extern const char *terminal_ui;
        extern const char *form_terminal_ui;
    } // namespace Conf
} // namespace View