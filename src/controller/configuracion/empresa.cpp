#include "controller/configuracion/empresa.hpp"

Empresa::Empresa(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VEmpresa(cobject, refBuilder) 
{
    signal_map().connect(sigc::mem_fun(*this, &Empresa::on_show_map));
    v_btn_guardar->signal_clicked().connect(sigc::mem_fun(*this, &Empresa::on_btn_clicked));
}

Empresa::~Empresa()
{
}

void Empresa::on_show_map()
{
    auto future = cpr::GetAsync(cpr::Url{Global::System::URL + "configuracion/get_informacion_empresa"});
    Global::Utility::consume_and_do(future, [this](const cpr::Response &response) {
        if (response.status_code == cpr::status::HTTP_OK) 
        {
            auto j = nlohmann::json::parse(response.text);
            
            auto razon = j["razon_social"].get<std::string>();
            auto direccion = j["direccion"].get<std::string>();
            auto rfc = j["rfc"].get<std::string>();
            auto contacto = j["contacto"].get<std::string>();
            auto agradecimiento = j["agradecimiento"].get<std::string>();

            v_datos_empresa[0]->set_text(razon);
            v_datos_empresa[1]->set_text(direccion);
            v_datos_empresa[2]->set_text(rfc);
            v_datos_empresa[3]->set_text(contacto);
            v_datos_empresa[4]->set_text(agradecimiento);
         }
    });
}

void Empresa::on_btn_clicked()
{
    auto json = nlohmann::json
    {
        {"razon_social", v_datos_empresa[0]->get_text()},
        {"direccion", v_datos_empresa[1]->get_text()},
        {"rfc", v_datos_empresa[2]->get_text()},
        {"contacto", v_datos_empresa[3]->get_text()},
        {"agradecimiento", v_datos_empresa[4]->get_text()}
    };
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "configuracion/actualiza_informacion_empresa"},
        cpr::Body{json.dump()}
    );

    Global::Utility::consume_and_do(future, [this](const cpr::Response &response) {
        if (response.status_code == cpr::status::HTTP_OK) 
        {
            
            Global::Widget::v_revealer->set_reveal_child(true);
            Global::Widget::v_revealer_title->set_text("Datos de empresa guardados con exito.");
        }
    });
}