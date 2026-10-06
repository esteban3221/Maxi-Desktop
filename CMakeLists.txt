cmake_minimum_required(VERSION 3.25)
project("Maxicajero Client Desktop" VERSION 2.1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -pthread") 
#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
#add_compile_options(-fno-declone-ctor-dtor)

message(STATUS "Proyecto: ${PROJECT_NAME}")
message(STATUS "Versión: ${PROJECT_VERSION}")
message(STATUS "Versión mayor: ${PROJECT_VERSION_MAJOR}")
message(STATUS "Versión menor: ${PROJECT_VERSION_MINOR}")
message(STATUS "Parche: ${PROJECT_VERSION_PATCH}")

# Configurar archivo de versión
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/configuracion/version.hpp
)

if (WIN32)
    set(APP_ICON_RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/icono.rc)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
endif()

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
    add_executable(Maxicajero ${SOURCES} ${APP_ICON_RESOURCE})
    target_link_libraries(Maxicajero
        PUBLIC
        ws2_32
        wldap32
        crypt32
        shlwapi
    )
else()
    add_executable(Maxicajero ${SOURCES})
endif()

if (UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CUPS REQUIRED cups)
    include_directories(${CUPS_INCLUDE_DIRS})
    target_link_libraries(Maxicajero PRIVATE ${CUPS_LIBRARIES})
endif()

target_link_libraries(Maxicajero 
    PUBLIC 
        Sqlite-cpp
        nlohmann_json::nlohmann_json 
        cpr::cpr
        ${GTKMM_LIBRARIES} 
        ${MINIZIP_LIBRARY}
        ${MZIP_LIBRARIES}
        ZLIB::ZLIB
        ixwebsocket::ixwebsocket  
        ${IXWEBSOCKET_LIBRARY}
        OpenSSL::SSL
        OpenSSL::Crypto
    PRIVATE
        Threads::Threads            
        -pthread                    
        stdc++                  
)

target_include_directories(Maxicajero PRIVATE ${GTKMM_INCLUDE_DIRS})

if (WIN32)
    find_program(OSSLSIGNCODE_EXECUTABLE osslsigncode)

    if (OSSLSIGNCODE_EXECUTABLE)
        message(STATUS "osslsigncode detectado: ${OSSLSIGNCODE_EXECUTABLE}")

        set(CERT_PATH "C:/Users/Esteban/Desktop/maxicajero_cert.pfx" CACHE FILEPATH "Ruta al certificado .pfx")
        set(CERT_PASS "1234" CACHE STRING "Contraseña del certificado")
        set(TSA_URL "http://timestamp.digicert.com" CACHE STRING "URL del Timestamp Authority")

        add_custom_command(
            TARGET Maxicajero POST_BUILD
            COMMAND ${OSSLSIGNCODE_EXECUTABLE} sign
                -pkcs12 "${CERT_PATH}"
                -pass "${CERT_PASS}"
                -t "${TSA_URL}"
                -in "$<TARGET_FILE:Maxicajero>"
                -out "$<TARGET_FILE_DIR:Maxicajero>/Maxicajero_signed.exe"
            COMMAND ${CMAKE_COMMAND} -E rename
                "$<TARGET_FILE_DIR:Maxicajero>/Maxicajero_signed.exe"
                "$<TARGET_FILE:Maxicajero>"
            COMMENT "Firmando ejecutable Maxicajero con osslsigncode..."
            VERBATIM
        )
    else()
        message(WARNING "osslsigncode no fue encontrado en el PATH. El ejecutable no será firmado.")
    endif()
endif()