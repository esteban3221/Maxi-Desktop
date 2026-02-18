#pragma once
#include <iostream>
#include <glibmm.h>
#include <giomm.h>
#include <memory>


#include "coneccion.hpp"

class MListIp : public Glib::Object
{
public:
    size_t m_id;
    Glib::ustring m_ip;
    bool m_selected = false;

    static Glib::RefPtr<MListIp> create(size_t id, const Glib::ustring &ip, bool selected = false)
    {
        return Glib::make_refptr_for_instance<MListIp>(new MListIp(id, ip, selected));
    }

protected:
    MListIp(size_t id, const Glib::ustring &ip, bool selected = false)
        : m_id(id), m_ip(ip), m_selected(selected)
    {
    }
};

class ListIp
{
private:
    /* data */
public:
    ListIp(/* args */);
    ~ListIp();
    
    Glib::RefPtr<Gio::ListStore<MListIp>> get_all_ip();
    size_t insert(const Glib::RefPtr<MListIp> &);
    void predeterminar(size_t);
    void delete_ip(size_t);
};