cmake_minimum_required(VERSION 3.25)
project(Backend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG") 

find_package(PkgConfig REQUIRED)
find_package(cpr REQUIRED)
find_package(nlohmann_json 3.11.3 REQUIRED)
pkg_check_modules(GTKMM gtkmm-4.0)
find_package(SQLite3 REQUIRED)

include_directories(
    ${GTKMM_INCLUDE_DIRS}
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
    src/view/bloq_num.cpp
    src/view/box_columns.cpp

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

    src/view/configuracion/general.cpp
    src/view/configuracion/sistema.cpp
    src/view/configuracion/empresa.cpp
    src/view/configuracion/usuarios.cpp
    src/view/configuracion/impresora.cpp
    # #================CONTROLLER=================
    src/controller/main_window.cpp
    src/controller/titlebar.cpp
    src/controller/login.cpp
    src/controller/configuracion.cpp 
    src/controller/movimientos.cpp 
    src/controller/pago_a.cpp 
    src/controller/pago_m.cpp 
    src/controller/refill.cpp 
    src/controller/retirada.cpp 
    src/controller/venta.cpp

    src/controller/configuracion/general.cpp
    src/controller/configuracion/sistema.cpp
    src/controller/configuracion/empresa.cpp
    src/controller/configuracion/usuarios.cpp
    src/controller/configuracion/impresora.cpp

    src/controller/configuracion/print_operation.cpp
    #================GLOBAL=================
    src/shared/global.cpp
)

include_directories(include 
                    include/controller
                    include/controller/accion
                    include/view
                    include/view/config
                    include/db
                    include/model
                    include/shared
                    ui
                    ui/menu
                    ui/componente
                    ui/conf

)

add_subdirectory(lib/Sqlit3-cpp-lib/)

add_executable(Maxicajero ${SOURCES})
if (UNIX AND NOT APPLE)  # Linux (no macOS)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CUPS REQUIRED cups)
    include_directories(${CUPS_INCLUDE_DIRS})
    target_link_libraries(Maxicajero PRIVATE ${CUPS_LIBRARIES})
endif()

target_link_libraries(Maxicajero 
                        PUBLIC 
                        Sqlite-cpp
                        nlohmann_json::nlohmann_json 
                        ${GTKMM_LIBRARIES} 
                        cpr::cpr
)
target_include_directories(Maxicajero PRIVATE ${GTKMM_INCLUDE_DIRS})
