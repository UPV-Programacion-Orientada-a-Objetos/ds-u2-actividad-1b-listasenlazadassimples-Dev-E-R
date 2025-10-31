cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(SistemaSensores VERSION 1.0)

# Especificar el estándar de C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Agregar opciones de compilación
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Definir el ejecutable
add_executable(program 
    main.cpp
)

# Incluir los archivos de encabezado
target_include_directories(program PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Mensaje de configuración
message(STATUS "Configurando Sistema IoT de Sensores")
message(STATUS "Compilador: ${CMAKE_CXX_COMPILER}")
message(STATUS "Estándar C++: ${CMAKE_CXX_STANDARD}")
