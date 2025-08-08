#include "view/refill.hpp"

VRefill::VRefill(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
																						  m_builder(refBuilder)
{
	v_tree_reciclador_billetes = m_builder->get_widget<Gtk::ColumnView>("treeRecicladorBilletes");
	v_tree_reciclador_monedas = m_builder->get_widget<Gtk::ColumnView>("treeRecicladorMonedas");

	v_lbl_total_parcial_monedas = m_builder->get_widget<Gtk::Label>("lblTotalMXN1");
	v_lbl_total_parcial_billetes = m_builder->get_widget<Gtk::Label>("lblTotalMXN2");
  v_lbl_total_billetes_cass = m_builder->get_widget<Gtk::Label>("lblTotalMXN2_");
	v_lbl_total_monedas = m_builder->get_widget<Gtk::Label>("lblTotalMXN4");
	v_lbl_total_billetes = m_builder->get_widget<Gtk::Label>("lblTotalMXN5");
	v_lbl_total = m_builder->get_widget<Gtk::Label>("lblTotalMXN3");

	v_btn_incia = m_builder->get_widget<Gtk::Button>("btn_iniciar");
	v_btn_transpaso = m_builder->get_widget<Gtk::Button>("btn_transpaso");
}

void VRefill::on_setup_label(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
	list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::END));
}

void VRefill::on_setup_button(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
	auto button = (Gtk::make_managed<Gtk::Button>());
	button->set_icon_name("document-save-symbolic");
	button->set_css_classes({"circular", "flat"});
	list_item->set_child(*button);
}

void VRefill::on_setup_spin(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
	auto spin = Gtk::make_managed<Gtk::SpinButton>(Gtk::Adjustment::create(0, 0, 100, 1, 10), 1.0, 0);
	spin->set_halign(Gtk::Align::FILL);
	list_item->set_child(*spin);
}

void VRefill::on_bind_deno(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
	auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
	auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

	label->set_text(Glib::ustring::format(col->m_denominacion));
}

void VRefill::on_bind_alm(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
	auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
	auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

	label->set_text(Glib::ustring::format(col->m_cant_alm));
}

void VRefill::on_bind_recy(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
	auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
	auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

	label->set_text(Glib::ustring::format(col->m_cant_recy));
}

void VRefill::on_bind_ingreso(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
	auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
	auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

	label->set_text(Glib::ustring::format(col->m_ingreso));
}

void VRefill::on_bind_inmo(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
	auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
	auto spin = dynamic_cast<Gtk::SpinButton *>(list_item->get_child());

	spin->set_adjustment(Gtk::Adjustment::create(col->m_nivel_inmo, 0, 100));
	spin->signal_changed().connect([this, col, spin](){ col->m_nivel_inmo = spin->get_value_as_int(); });
	spin->signal_value_changed().connect([this, col, spin](){ col->m_nivel_inmo = spin->get_value_as_int(); });
}

void VRefill::on_bind_inmo_min(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
	auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
	auto spin = dynamic_cast<Gtk::SpinButton *>(list_item->get_child());

	spin->set_adjustment(Gtk::Adjustment::create(col->m_nivel_inmo_min, 0, 100));
	spin->signal_changed().connect([this, col, spin](){ col->m_nivel_inmo_min = spin->get_value_as_int(); });
	spin->signal_value_changed().connect([this, col, spin](){ col->m_nivel_inmo_min = spin->get_value_as_int(); });
}

void VRefill::on_bind_inmo_max(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
	auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
	auto spin = dynamic_cast<Gtk::SpinButton *>(list_item->get_child());

	spin->set_adjustment(Gtk::Adjustment::create(col->m_nivel_inmo_max, 0, 100));
	spin->signal_changed().connect([this, col, spin](){ col->m_nivel_inmo_max = spin->get_value_as_int(); });
	spin->signal_value_changed().connect([this, col, spin](){ col->m_nivel_inmo_max = spin->get_value_as_int(); });
}

void VRefill::on_bind_btn(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
	auto col = std::dynamic_pointer_cast<MLevelCash>(list_item->get_item());
	auto button = dynamic_cast<Gtk::Button *>(list_item->get_child());

	button->signal_clicked().connect([this, col]()
	{

      auto json = nlohmann::json
      {
          {"denominacion", col->m_denominacion},
          {"nivel_inmo", col->m_nivel_inmo},
          {"nivel_inmo_min", col->m_nivel_inmo_min},
          {"nivel_inmo_max", col->m_nivel_inmo_max}
      };
      cpr::Body body = json.dump();

      auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "validadores/update_imovilidad"}, Global::Utility::header, body);

        Global::Utility::consume_and_do(future,[this](cpr::Response response)
        {
            if (response.status_code == 200) 
                Global::Widget::reveal_toast("Exito");
            else
                std::cerr << "Error: " << response.status_code << std::endl;
        }); 
  	});
}

VRefill::~VRefill()
{
}

namespace View
{
	const char *refill_ui = R"(<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_refill">
    <property name="halign">0</property>
    <property name="margin-bottom">15</property>
    <property name="margin-end">15</property>
    <property name="margin-start">15</property>
    <property name="margin-top">15</property>
    <property name="spacing">20</property>
    <child>
      <object class="GtkBox">
        <property name="homogeneous">true</property>
        <property name="orientation">1</property>
        <property name="spacing">20</property>
        <property name="vexpand">true</property>
        <child>
          <object class="GtkFrame" id="frmRecicladorMonedas">
            <child>
              <object class="GtkBox">
                <property name="margin-end">12</property>
                <property name="margin-start">12</property>
                <property name="orientation">1</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Monedas</property>
                    <style>
                      <class name="title-1"/>
                    </style>
                  </object>
                </child>
                <child>
                  <object class="GtkBox">
                    <property name="margin-bottom">12</property>
                    <property name="margin-top">12</property>
                    <property name="spacing">10</property>
                    <property name="vexpand">true</property>
                    <child>
                      <object class="GtkColumnView" id="treeRecicladorMonedas">
                        <property name="enable-rubberband">true</property>
                        <property name="hexpand">true</property>
                        <property name="hexpand-set">true</property>
                        <property name="show-column-separators">true</property>
                        <property name="vexpand">true</property>
                        <property name="vexpand-set">true</property>
                      </object>
                    </child>
                    <child>
                      <object class="GtkBox">
                        <property name="halign">2</property>
                        <property name="hexpand-set">true</property>
                        <property name="orientation">1</property>
                        <child>
                          <object class="GtkLabel">
                            <property name="label">Total
Recyclador</property>
                            <style>
                              <class name="title-4"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkLabel" id="lblTotalMXN1">
                            <property name="label">0.00 MXN</property>
                            <style>
                              <class name="title-4"/>
                              <class name="dim-label"/>
                            </style>
                          </object>
                        </child>
                      </object>
                    </child>
                  </object>
                </child>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkFrame" id="frmRecicladorBilletes">
            <child>
              <object class="GtkBox">
                <property name="margin-end">12</property>
                <property name="margin-start">12</property>
                <property name="orientation">1</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Billetes</property>
                    <style>
                      <class name="title-1"/>
                    </style>
                  </object>
                </child>
                <child>
                  <object class="GtkBox">
                    <property name="margin-bottom">12</property>
                    <property name="margin-top">12</property>
                    <property name="spacing">10</property>
                    <child>
                      <object class="GtkColumnView" id="treeRecicladorBilletes">
                        <property name="enable-rubberband">true</property>
                        <property name="hexpand">true</property>
                        <property name="hexpand-set">true</property>
                        <property name="show-column-separators">true</property>
                        <property name="vexpand">true</property>
                        <property name="vexpand-set">true</property>
                      </object>
                    </child>
                    <child>
                      <object class="GtkBox">
                        <property name="halign">2</property>
                        <property name="orientation">1</property>
                        <child>
                          <object class="GtkLabel">
                            <property name="label">Total
Recyclador</property>
                            <style>
                              <class name="title-4"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkLabel" id="lblTotalMXN2">
                            <property name="label">0.00 MXN</property>
                            <property name="margin-bottom">10</property>
                            <style>
                              <class name="title-4"/>
                              <class name="dim-label"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkLabel">
                            <property name="label">Total
Casette      </property>
                            <style>
                              <class name="title-4"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkLabel" id="lblTotalMXN2_">
                            <property name="label">0.00 MXN</property>
                            <style>
                              <class name="title-4"/>
                              <class name="dim-label"/>
                            </style>
                          </object>
                        </child>
                      </object>
                    </child>
                  </object>
                </child>
              </object>
            </child>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkBox">
        <property name="orientation">1</property>
        <property name="spacing">20</property>
        <child>
          <object class="GtkFrame" id="frmTotal">
            <child>
              <object class="GtkBox">
                <property name="margin-bottom">12</property>
                <property name="margin-end">12</property>
                <property name="margin-start">12</property>
                <property name="margin-top">12</property>
                <property name="orientation">1</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Total</property>
                    <style>
                      <class name="title-1"/>
                    </style>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel" id="lblTotalMXN3">
                    <property name="label">0.00 MXN</property>
                    <style>
                      <class name="title-2"/>
                    </style>
                  </object>
                </child>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkFrame" id="frmTotal2">
            <child>
              <object class="GtkBox">
                <property name="margin-bottom">12</property>
                <property name="margin-end">12</property>
                <property name="margin-start">12</property>
                <property name="margin-top">12</property>
                <property name="orientation">1</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Total Monedas</property>
                    <style>
                      <class name="title-1"/>
                    </style>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel" id="lblTotalMXN4">
                    <property name="label">0.00 MXN</property>
                    <style>
                      <class name="title-2"/>
                    </style>
                  </object>
                </child>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkFrame" id="frmTotal3">
            <child>
              <object class="GtkBox">
                <property name="margin-bottom">12</property>
                <property name="margin-end">12</property>
                <property name="margin-start">12</property>
                <property name="margin-top">12</property>
                <property name="orientation">1</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Total Billetes</property>
                    <style>
                      <class name="title-1"/>
                    </style>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel" id="lblTotalMXN5">
                    <property name="label">0.00 MXN</property>
                    <style>
                      <class name="title-2"/>
                    </style>
                  </object>
                </child>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_iniciar">
            <property name="css-classes">suggested-action</property>
            <property name="label" translatable="yes">Iniciar Refill / Carga</property>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_transpaso">
            <property name="label">Transpasar a Casette</property>
          </object>
        </child>
      </object>
    </child>
  </object>
</interface>)";
} // namespace View