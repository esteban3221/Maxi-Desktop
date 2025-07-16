#pragma once
#include <gtkmm.h>
#include "global.hpp"
#include "bloq_num.hpp"
#include "box_columns.hpp"

class VPago : public Gtk::Box
{
private:

protected:
    VBaseNip *v_base_nip = nullptr;

    std::unique_ptr<Gtk::MessageDialog> v_dialog;
public:
    VPago();
    ~VPago();
};


// namespace View
// {
//     extern const char *pagoa_ui;
// } // namespace View