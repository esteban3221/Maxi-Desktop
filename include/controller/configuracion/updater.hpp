#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <cstdlib>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif
#include "configuracion/version.hpp"
#include "coneccion.hpp"
#include "global.hpp"

void checkAndApplyUpdate()
{
    try
    {
        auto &database = Database::getInstance();
        auto result = database.sqlite3->command("SELECT valor FROM configuracion WHERE id = 101 AND id = 105");

        std::string URL = result->at("valor")[0];     // Asumiendo que la URL está en la primera fila y columna
        std::string CHANNEL = result->at("valor")[1]; // Puede ser "lts" o "testing"
        std::string url_manifest = "https://" + URL + "/updates/" + CHANNEL + "/windows/latest.json";

        auto response = cpr::Get(cpr::Url{url_manifest});
        if (response.status_code != 200)
        {
            Global::Widget::reveal_toast("No se pudo contactar al servidor de actualizaciones.");
            return;
        }

        auto json_data = nlohmann::json::parse(response.text);
        int remote_major = json_data["major"];
        int remote_minor = json_data["minor"];
        int remote_patch = json_data["patch"];
        int remote_build = json_data["build"];
        std::string download_url = json_data["url"];

        bool hay_actualizacion = false;
        if (remote_major > Maxicajero::Version::MAJOR)
            hay_actualizacion = true;
        else if (remote_major == Maxicajero::Version::MAJOR && remote_minor > Maxicajero::Version::MINOR)
            hay_actualizacion = true;
        else if (remote_major == Maxicajero::Version::MAJOR && remote_minor == Maxicajero::Version::MINOR && remote_patch > Maxicajero::Version::PATCH)
            hay_actualizacion = true;
        else if (remote_major == Maxicajero::Version::MAJOR && remote_minor == Maxicajero::Version::MINOR)
            hay_actualizacion = true;

        if (!hay_actualizacion)
        {
            Global::Widget::reveal_toast("El sistema está actualizado.");
            return;
        }

#if defined(__linux__)
        Global::Widget::reveal_toast("Actualización disponible, por favor descargue la nueva versión desde el sistema de actualizaciones o desde la página oficial.");
#endif

        Global::Widget::reveal_toast("Nueva versión encontrada: " + std::to_string(remote_major) + "." + std::to_string(remote_minor) + "." + std::to_string(remote_patch) + " - Descargando actualización...");

#if defined(_WIN32) || defined(_WIN64)
        char temp_dir[MAX_PATH];
        GetTempPathA(MAX_PATH, temp_dir);
        std::string temp_path = std::string(temp_dir) + "Maxicajero-new.exe";

        cpr::Session session;
        session.SetUrl(cpr::Url{download_url});
        std::ofstream file(temp_path, std::ios::binary);
        session.SetWriteCallback(cpr::WriteCallback([&file](std::string data, int)
                                                    {
            file.write(data.c_str(), data.size());
            return true; }));
        auto download_response = session.Get();

        if (download_response.status_code != 200)
        {
            Global::Widget::reveal_toast("Error al descargar la actualización.", (Gtk::MessageType)(3));
            return;
        }
        file.close();

        // 4. Obtener la ruta absoluta del ejecutable actual de Windows
        char current_path[MAX_PATH];
        GetModuleFileNameA(NULL, current_path, MAX_PATH);
        std::string current_exe(current_path);

        // 5. Crear un archivo por lotes (.bat) temporal para realizar el reemplazo seguro
        std::string batch_path = std::string(temp_dir) + "update.bat";
        std::ofstream batch_file(batch_path);

        batch_file << "@echo off\n";
        batch_file << "timeout /t 2 /nobreak > nul\n"                               // Espera a que el proceso principal muera por completo
                   << "move /y \"" << temp_path << "\" \"" << current_exe << "\"\n" // Sobrescribe el .exe viejo
                   << "start \"\" \"" << current_exe << "\"\n"                      // Vuelve a abrir el programa actualizado
                   << "del \"%~f0\"\n";                                             // Borra este script automáticamente
        batch_file.close();

        Global::Widget::reveal_toast("Aplicando actualización y reiniciando...");

        // 6. Ejecutar el script .bat en segundo plano de forma oculta y cerrar la app actual
        ShellExecuteA(NULL, "open", batch_path.c_str(), NULL, NULL, SW_HIDE);

        std::exit(0);
#endif
    }
    catch (const std::exception &e)
    {
        Global::Widget::reveal_toast("Excepción en el actualizador: " + std::string(e.what()), (Gtk::MessageType)(3));
    }
}