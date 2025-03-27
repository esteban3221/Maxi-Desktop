#pragma once
#include "view/refill.hpp"


class Refill : public VRefill
{
private:
    void on_show_map();
    void init_data(Gtk::ColumnView *vcolumn,const Glib::RefPtr<Gio::ListStore<MLevelCash>>& level);
    void actualiza_data(const Glib::RefPtr<Gtk::SelectionModel> &selection,const Glib::RefPtr<Gio::ListStore<MLevelCash>>& level);
    void safe_clear_column_view(Gtk::ColumnView* column_view);
public: 
    Refill(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Refill();
};