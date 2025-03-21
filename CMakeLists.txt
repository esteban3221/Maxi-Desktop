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
    src/view/titlebar.cpp
    src/view/login.cpp
    src/view/menu.cpp
    # #================CONTROLLER=================
    src/controller/main_window.cpp
    src/controller/titlebar.cpp
    src/controller/login.cpp
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
)

add_subdirectory(lib/Sqlit3-cpp-lib/)

add_executable(Maxicajero ${SOURCES})

target_link_libraries(Maxicajero 
                        PUBLIC 
                        Sqlite-cpp
                        nlohmann_json::nlohmann_json 
                        ${GTKMM_LIBRARIES} 
                        cpr::cpr
)
target_include_directories(Maxicajero PRIVATE ${GTKMM_INCLUDE_DIRS})
