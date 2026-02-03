#pragma once
#include <gtkmm.h>

class VTerminal : public Gtk::Box
{
    private:
    Glib::RefPtr<Gtk::Builder> m_builder;

    protected:
    Gtk::Button *v_btn_add_terminal{nullptr};
    Gtk::Button *v_btn_close_rvl{nullptr};
    Gtk::Label *v_lbl_titulo_rvl{nullptr};
    Gtk::FlowBox *v_flow_terminals{nullptr};
    Gtk::Revealer *v_rvl{nullptr};

    public:
    VTerminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VTerminal();
};

namespace View
{
    namespace Conf
    {
        extern const char *terminal_ui;
    } // namespace Conf
} // namespace View