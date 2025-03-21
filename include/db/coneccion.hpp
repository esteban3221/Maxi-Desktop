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
                                   "ip TEXT NOT NULL UNIQUE\n"
                                   ")");
            
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