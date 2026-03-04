#include "view/pago_m.hpp"

VPagoM::VPagoM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                        m_builder(refBuilder)
{
  v_btn_cobrar = m_builder->get_widget<Gtk::Button>("btn_cobrar");
  v_ety_concepto = m_builder->get_widget<Gtk::Entry>("ety_concepto");
  v_box_level_validadores = m_builder->get_widget<Gtk::Box>("box_level_validadores");

  Global::Utility::set_multiline_text(*v_ety_concepto);

  auto css_provider = Gtk::CssProvider::create();
  css_provider->load_from_data(R"(
    .card-pago {
        background-color: rgba(255, 255, 255, 0.05);
        border: 1px solid #444;
        border-radius: 12px;
    }

    .card-pago label.title {
        font-weight: bold;
        color: #2ecc71; 
    }

    .denom-label {
        font-size: 1.1rem;
        font-weight: 500;
    }

    spinbutton {
        min-height: 40px;
        min-width: 120px;
    }
    )"
  );

  Gtk::StyleContext::add_provider_for_display(
      Gdk::Display::get_default(),
      css_provider,
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

VPagoM::~VPagoM()
{
}

Gtk::Box *VPagoM::agregar_contenedor(const std::vector<std::pair<size_t, size_t>> &vec_val, std::string key)
{
  // El Frame le da un borde y un título al grupo (Billetero o Monedero)
  auto name_frame = key;
  name_frame.erase(name_frame.find_first_of('-'), name_frame.size());
  std::replace(name_frame.begin(), name_frame.end(), '_', ' ');
  auto frame = Gtk::make_managed<Gtk::Frame>(name_frame);
  auto root = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 12);

  root->set_margin(15); // Espacio interno
  frame->set_child(*root);
  frame->set_margin(10);
  frame->set_css_classes({"card-pago"}); // Clase personalizada para CSS

  auto &lista_spins = m_inputs_pago[key];
  lista_spins.clear();

  for (auto &&i : vec_val)
  {
    auto row = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 0);

    // Label con ancho fijo para que todos los Spins queden alineados verticalmente
    auto label = Gtk::make_managed<Gtk::Label>(Glib::ustring::compose("$ %1", i.first));
    label->set_width_chars(6);
    label->set_halign(Gtk::Align::START);
    label->set_css_classes({"denom-label"});

    auto spin = Gtk::make_managed<Gtk::SpinButton>(Gtk::Adjustment::create(0, 0, i.second, 1, 10, 0));
    spin->set_numeric(true);
    spin->set_halign(Gtk::Align::END);
    spin->set_hexpand(true);

    lista_spins.push_back({spin, (int)i.first});

    row->append(*label);
    row->append(*spin);
    root->append(*row);
  }

  // Retornamos el frame en lugar del box para que incluya el borde y título
  auto wrapper = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
  wrapper->append(*frame);
  wrapper->set_hexpand(true);
  return wrapper;
}

namespace View
{
  const char *pagom_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.96.3 -->
<interface>
  <!-- interface-name pago_m.ui -->
  <requires lib="gtk" version="4.12"/>
  <object class="GtkBox" id="box_pago_m">
    <property name="orientation">vertical</property>
    <property name="spacing">10</property>
    <child>
      <object class="GtkEntry" id="ety_concepto">
        <property name="halign">3</property>
        <property name="margin-top">10</property>
        <property name="margin-bottom">20</property>
        <property name="width-request">500</property>
        <property name="height-request">50</property>
        <property name="placeholder-text" translatable="yes">Concepto</property>
        <style>
          <class name="title-1"/>
        </style>
      </object>
    </child>
    <child>
      <object class="GtkBox" id="box_level_validadores">
        <property name="homogeneous">True</property>
        <property name="spacing">20</property>
      </object>
    </child>
    <child>
      <object class="GtkButton" id="btn_cobrar">
        <property name="halign">center</property>
        <property name="hexpand">True</property>
        <property name="label">Cobrar $0</property>
        <property name="valign">baseline-center</property>
        <property name="vexpand">True</property>
        <style>
          <class name="suggested-action"/>
        </style>
      </object>
    </child>
  </object>
</interface>
)";
} // namespace View