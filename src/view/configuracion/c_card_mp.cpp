#include "view/configuracion/c_card_mp.hpp"

VCardMP::VCardMP(const Glib::ustring& titulo,
                 const Glib::ustring& icon_name,
                 bool mnemonic)
{
    m_content_box.set_margin(12);
    m_content_box.set_spacing(10);
    set_child(m_content_box);
    set_halign(Gtk::Align::CENTER);
    set_valign(Gtk::Align::START);

    m_lbl_titulo = Gtk::make_managed<Gtk::Label>();
    m_lbl_titulo->set_markup("<b><big>" + Glib::Markup::escape_text(titulo) + "</big></b>");
    m_lbl_titulo->set_halign(Gtk::Align::CENTER);
    m_lbl_titulo->set_ellipsize(Pango::EllipsizeMode::MIDDLE);
    m_content_box.append(*m_lbl_titulo);

    m_lbl_tipo = Gtk::make_managed<Gtk::Label>();
    m_lbl_tipo->add_css_class("caption");
    m_lbl_tipo->set_opacity(0.7);
    m_lbl_tipo->set_halign(Gtk::Align::START);
    m_content_box.append(*m_lbl_tipo);

    m_sep1 = Gtk::make_managed<Gtk::Separator>(Gtk::Orientation::HORIZONTAL);
    m_content_box.append(*m_sep1);

    m_lbl_alias = Gtk::make_managed<Gtk::Label>();
    m_lbl_alias->add_css_class("heading");   // o "title" según tu tema
    m_lbl_alias->set_ellipsize(Pango::EllipsizeMode::END);
    m_lbl_alias->set_max_width_chars(25);
    m_lbl_alias->set_halign(Gtk::Align::CENTER);
    m_content_box.append(*m_lbl_alias);

    m_image = Gtk::make_managed<Gtk::Image>();
    m_image->set_from_icon_name(icon_name);
    m_image->set_pixel_size(64);               // tamaño recomendado para tarjetas
    m_image->set_halign(Gtk::Align::CENTER);
    m_image->set_valign(Gtk::Align::CENTER);
    m_image->set_hexpand(false);
    m_image->set_vexpand(false);
    m_content_box.append(*m_image);

    m_sep2 = Gtk::make_managed<Gtk::Separator>(Gtk::Orientation::HORIZONTAL);
    m_content_box.append(*m_sep2);

    m_lbl_descripcion = Gtk::make_managed<Gtk::Label>();
    m_lbl_descripcion->add_css_class("dim-label");
    m_lbl_descripcion->set_wrap(true);
    m_lbl_descripcion->set_wrap_mode(Pango::WrapMode::WORD_CHAR);
    m_lbl_descripcion->set_justify(Gtk::Justification::CENTER);
    m_lbl_descripcion->set_max_width_chars(30);
    m_content_box.append(*m_lbl_descripcion);
}

VCardMP::~VCardMP()
{
    // No es necesario liberar nada porque usamos make_managed
}

void VCardMP::set_tipo(const Glib::ustring& text)
{
    if (m_lbl_tipo) m_lbl_tipo->set_text(text);
}

void VCardMP::set_alias(const Glib::ustring& text)
{
    if (m_lbl_alias) m_lbl_alias->set_text(text);
}

void VCardMP::set_descripcion(const Glib::ustring& text)
{
    if (m_lbl_descripcion) m_lbl_descripcion->set_text(text);
}

void VCardMP::set_icon_name(const Glib::ustring& icon_name)
{
    if (m_image) m_image->set_from_icon_name(icon_name);
}