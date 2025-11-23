#pragma once
#include "view/configuracion/general.hpp"
#include "model/configuracion.hpp"
#include "controller/configuracion/impresora.hpp"
#include "minizip/zip.h"

#include <filesystem>

class General : public VGeneral
{
private:
    void on_show_mapping();
    void on_ety_change_mensaje_inicio();
    void on_btn_reinicia_val_clicked();
    void on_btn_retirada();

    void on_btn_actualiza_pos_clicked();
    void on_btn_imagen_pos_clicked();
    void on_btn_imagen_2_clicked();
    void on_btn_desactiva_carrousel_clicked();
    void on_file_dialog_finish(const Glib::RefPtr<Gio::AsyncResult>& result,const Glib::RefPtr<Gtk::FileDialog>& dialog);
    void on_file_dialog_image_finish(const Glib::RefPtr<Gio::AsyncResult>& result,const Glib::RefPtr<Gtk::FileDialog>& dialog);
    void on_folder_dialog_finish(const Glib::RefPtr<Gio::AsyncResult>& result, const Glib::RefPtr<Gtk::FileDialog>& dialog);
    void on_chk_mostrar_notificaciones_toggled();
    bool comprimir_carpeta(const std::string& carpeta_origen, const std::string& archivo_zip);
    bool agregar_archivo_a_zip(zipFile zf, const std::string& ruta_archivo, const std::string& nombre_en_zip);
public:
    General(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~General();
};