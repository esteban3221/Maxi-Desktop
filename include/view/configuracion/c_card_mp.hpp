#pragma once
#include <gtkmm.h>

class VCardMP : public Gtk::Button
{
public:
    VCardMP(const Glib::ustring& titulo,
            const Glib::ustring& icon_name,
            bool mnemonic = false);

    virtual ~VCardMP();

    // Métodos para actualizar contenido dinámicamente
    void set_tipo(const Glib::ustring& text);
    void set_alias(const Glib::ustring& text);
    void set_descripcion(const Glib::ustring& text);
    void set_icon_name(const Glib::ustring& icon_name);

private:
    Gtk::Box           m_content_box {Gtk::Orientation::VERTICAL, 8};
    Gtk::Label*        m_lbl_titulo    {nullptr};
    Gtk::Label*        m_lbl_tipo      {nullptr};
    Gtk::Label*        m_lbl_alias     {nullptr};
    Gtk::Label*        m_lbl_descripcion{nullptr};
    Gtk::Image*        m_image         {nullptr};
    Gtk::Separator*    m_sep1          {nullptr};
    Gtk::Separator*    m_sep2          {nullptr};
};