cmake_minimum_required(VERSION 3.26)
project(lab1)

set(CMAKE_CXX_STANDARD 17)

find_package(ODBC REQUIRED)

include_directories(${ODBC_INCLUDE_DIRS})
link_directories(${ODBC_LIBRARY_DIRS})

add_executable(lab1 main.cpp
        utils/config_reader.h
        models/pharmacy.h
        utils/sql_executor.h
        utils/menu.h
        utils/table_printer.h
        models/medicine.h
        models/request.h
        models/date.h
        models/medicine_buying.h
        models/manufacturer.h
        models/country.h
)

target_link_libraries(lab1 ${ODBC_LIBRARIES})