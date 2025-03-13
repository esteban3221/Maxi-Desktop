#pragma once
#include "view/titlebar.hpp"

class TitleBar : public VTitlebar
{
private:
    /* data */
public:
    TitleBar(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~TitleBar();
};