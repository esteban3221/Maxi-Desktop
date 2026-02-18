#pragma once
#include "sqlite.hpp"
#include <memory>

// Singleton de base de datos
class Database
{
private:
    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;

    ~Database(void) {}

    Database(void)
    {
        sqlite3 = std::make_unique<SQLite3::SQLite>("data.db");
        this->sqlite3->open();

        if (not this->sqlite3->is_created())
        {
            this->sqlite3->command("CREATE TABLE ip (\n"
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
                                   "ip TEXT NOT NULL UNIQUE,\n"
                                   "predeterminado INTEGER NOT NULL DEFAULT 0\n"
                                   ")");

            this->sqlite3->command("CREATE TABLE configuracion (\n"
                                    "id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
                                    "descripcion TEXT NOT NULL,\n"
                                    "valor TEXT NOT NULL\n"
                                    ")");
            
            this->sqlite3->command("INSERT into configuracion values"
                                   // 1 - 3
                                   "(1,'Mostrar Notificacion','1'),"
                                   "(2,'Mensaje Inicio','Bienvenido'),"
                                   "(3,'Pagos Superiores','10000'),"
                                   // 4 - 11
                                   "(4,'Impresora default',''),"
                                   "(5,'Activa Impresion','1'),"
                                   "(6,'Visualiza Agradecimineto','1'),"
                                   "(7,'Visualiza Fecha','1'),"
                                   "(8,'Visualiza Direccion','1'),"
                                   "(9,'Visualiza RFC','1'),"
                                   "(10,'Visualiza Vendedor','1'),"
                                   "(11,'Visualiza Contacto','1')");
            
        }
    }

public:
    std::unique_ptr<SQLite3::SQLite> sqlite3;
    static Database &getInstance()
    {
        static Database instance; // Instancia única de la clase
        return instance;
    }
};