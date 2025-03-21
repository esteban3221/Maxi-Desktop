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

    static Glib::RefPtr<MListIp> create(size_t id, const Glib::ustring &ip)
    {
        return Glib::make_refptr_for_instance<MListIp>(new MListIp(id, ip));
    }

protected:
    MListIp(size_t id, const Glib::ustring &ip)
        : m_id(id), m_ip(ip)
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
    void delete_ip(const Glib::RefPtr<MListIp> &);
};