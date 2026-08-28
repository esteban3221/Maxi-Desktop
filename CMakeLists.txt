cmake_minimum_required(VERSION 3.25)
project("Maxicajero Client Desktop" VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fPIC") 
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

message(STATUS "Proyecto: ${PROJECT_NAME}")
message(STATUS "Versión: ${PROJECT_VERSION}")

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/configuracion/version.hpp
)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(cpr REQUIRED)
find_package(ZLIB REQUIRED)
find_package(ixwebsocket REQUIRED)
pkg_check_modules(MZIP minizip REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json 3.11.3 REQUIRED)
pkg_check_modules(GTKMM gtkmm-4.0)
find_package(SQLite3 REQUIRED)

include_directories(
    ${GTKMM_INCLUDE_DIRS}
    ${MZIP_LIBDIR}
    ${IXWEBSOCKET_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(SOURCES
    src/main.cpp
    #===================MODEL===================
    src/model/log.cpp
    src/model/level_cash.cpp
    src/model/log_historial.cpp
    src/model/pago_pendiente.cpp
    src/model/roles.cpp
    src/model/usuario_historial.cpp
    src/model/usuarios_roles.cpp
    src/model/usuarios.cpp
    src/model/configuracion.cpp
    src/model/list_ip.cpp
    #===================VIEW====================
    src/view/titlebar.cpp
    src/view/login.cpp
    src/view/menu.cpp
    src/view/venta.cpp 
    src/view/retirada.cpp 
    src/view/refill.cpp 
    src/view/pago_m.cpp 
    src/view/pago_a.cpp
    src/view/movimientos.cpp 
    src/view/configuracion.cpp
    src/view/cambio.cpp

    src/view/configuracion/general.cpp
    src/view/configuracion/sistema.cpp
    src/view/configuracion/empresa.cpp
    src/view/configuracion/usuarios.cpp
    src/view/configuracion/impresora.cpp

    #===============COMPONENTES================
    src/view/bloq_num.cpp
    src/view/box_columns.cpp
    src/view/configuracion/c_card_mp.cpp
    src/view/configuracion/terminal.cpp

    # #================CONTROLLER=================
    src/controller/main_window.cpp
    src/controller/titlebar.cpp
    src/controller/login.cpp
    src/controller/configuracion.cpp 
    src/controller/movimientos.cpp 
    src/controller/pago_a.cpp 
    src/controller/pago_m.cpp 
    src/controller/cambio_a.cpp 
    src/controller/cambio_m.cpp 
    src/controller/refill.cpp 
    src/controller/retirada.cpp 
    src/controller/venta.cpp
    src/controller/cambio.cpp

    src/controller/configuracion/general.cpp
    src/controller/configuracion/sistema.cpp
    src/controller/configuracion/empresa.cpp
    src/controller/configuracion/usuarios.cpp
    src/controller/configuracion/impresora.cpp
    src/controller/configuracion/terminal.cpp

    src/controller/configuracion/print_operation.cpp
    #================GLOBAL=================
    src/shared/global.cpp
)

include_directories(include 
                    include/controller
                    include/controller/accion
                    include/view
                    include/view/config
                    include/view/configuracion/
                    include/db
                    include/model
                    include/shared
                    ui
                    ui/menu
                    ui/componente
                    ui/conf
)

add_subdirectory(lib/Sqlit3-cpp-lib/)

if(WIN32)
    set(APP_ICON_RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/icono.rc)
    add_executable(Maxicajero ${SOURCES} ${APP_ICON_RESOURCE})
    set_target_properties(Maxicajero PROPERTIES LINK_FLAGS "-mwindows")
    add_executable(Maxicajero-Debug ${SOURCES} ${APP_ICON_RESOURCE})

    foreach(TARGET_NAME Maxicajero Maxicajero-Debug)
        target_link_libraries(${TARGET_NAME}
            PUBLIC
            ws2_32
            wldap32
            crypt32
        )
    endforeach()
else()
    add_executable(Maxicajero ${SOURCES})
endif()

if (UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CUPS REQUIRED cups)
    include_directories(${CUPS_INCLUDE_DIRS})
    target_link_libraries(Maxicajero PRIVATE ${CUPS_LIBRARIES})
endif()

if(WIN32)
    foreach(TARGET_NAME Maxicajero Maxicajero-Debug)
        target_link_libraries(${TARGET_NAME} 
            PUBLIC 
            Sqlite-cpp
            nlohmann_json::nlohmann_json 
            ixwebsocket::ixwebsocket
            OpenSSL::SSL
            OpenSSL::Crypto
            ${GTKMM_LIBRARIES} 
            ${MINIZIP_LIBRARY}
            ${MZIP_LIBRARIES}
            ${IXWEBSOCKET_LIBRARY}
            ZLIB::ZLIB
            cpr::cpr
        )
        target_include_directories(${TARGET_NAME} PRIVATE ${GTKMM_INCLUDE_DIRS})
    endforeach()
else()
    target_link_libraries(Maxicajero 
        PUBLIC 
        Sqlite-cpp
        nlohmann_json::nlohmann_json 
        ixwebsocket::ixwebsocket
        OpenSSL::SSL
        OpenSSL::Crypto
        ${GTKMM_LIBRARIES} 
        ${MINIZIP_LIBRARY}
        ${MZIP_LIBRARIES}
        ${IXWEBSOCKET_LIBRARY}
        ZLIB::ZLIB
        cpr::cpr
    )
    target_include_directories(Maxicajero PRIVATE ${GTKMM_INCLUDE_DIRS})
endif()