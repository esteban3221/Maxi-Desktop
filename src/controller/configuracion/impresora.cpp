#include "controller/configuracion/impresora.hpp"

Impresora::Impresora(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VImpresora(cobject, refBuilder) 
{
    #ifdef __linux__
        signal_map().connect(sigc::mem_fun(*this, &Impresora::init_impresoras_linux));
    #endif

    v_list_box_print->signal_row_activated().connect(
        sigc::mem_fun(*this, &Impresora::on_list_box_row_selected));
}

Impresora::~Impresora()
{
}

void Impresora::init_impresoras_linux()
{
    cups_dest_t *printers;
    int num_printers = cupsGetDests(&printers);

    if (num_printers > 0)
    {
        v_list_box_print->remove_all();
        auto db_impresora = std::make_unique<Configuracion>();
        auto db_contendor = db_impresora->get_conf_data(4, 4);
        std::string impresora_default = db_contendor->get_item(0)->m_valor;

        for (int i = 0; i < num_printers; i++)
        {
            std::string name = printers[i].name;
            std::string subtitle = cupsGetOption("device-uri", printers[i].num_options, printers[i].options);
            if (subtitle.empty())
                subtitle = "None";
            auto printer = Gtk::make_managed<VListPrinters>(name, subtitle);
            if (name == impresora_default)
                printer->v_image_check->set_opacity(1);
            
            v_list_box_print->append(*printer);
        }
    } 
    
    cupsFreeDests(num_printers, printers);
}

void Impresora::on_list_box_row_selected(Gtk::ListBoxRow *row)
{
    auto this_row = dynamic_cast<VImpresora::VListPrinters *>(row);
    for (auto &&i : v_list_box_print->get_children())
    {
        auto printer = dynamic_cast<VListPrinters *>(i);
        if (printer)
        {
            printer->v_image_check->set_opacity(0);
            if (printer == this_row)
                printer->v_image_check->set_opacity(1);
        }
    }
}