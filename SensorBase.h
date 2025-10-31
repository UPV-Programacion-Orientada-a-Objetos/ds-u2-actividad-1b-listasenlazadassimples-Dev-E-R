/**
 * @file SensorBase.h
 * @brief Clase base abstracta para todos los sensores del sistema
 * @author Sistema de Monitoreo
 * @version 1.0
 * @date 2024
 */

#ifndef SENSORBASE_H
#define SENSORBASE_H

#include <iostream>
#include <cstring>

/**
 * @class SensorBase
 * @brief Clase abstracta fundamental para dispositivos de medicion
 * 
 * Establece la interfaz comun que deben implementar todos los sensores
 * del sistema. Utiliza polimorfismo para permitir el manejo uniforme
 * de diferentes tipos de sensores.
 */
class SensorBase {
protected:
    char nombre[50];  ///< Cadena identificadora del dispositivo
    
public:
    /**
     * @brief Constructor parametrizado
     * @param identificador Nombre o codigo del sensor (maximo 49 caracteres)
     * 
     * Inicializa el sensor con un identificador unico.
     */
    SensorBase(const char* identificador = "DISPOSITIVO") {
        std::strncpy(nombre, identificador, 49);
        nombre[49] = '\0';
    }
    
    /**
     * @brief Destructor virtual
     * 
     * Permite la correcta destruccion de objetos derivados mediante
     * punteros a la clase base.
     */
    virtual ~SensorBase() {
        std::cout << "[Eliminacion] Dispositivo finalizado: " << nombre << std::endl;
    }
    
    /**
     * @brief Metodo virtual puro para procesamiento de lecturas
     * 
     * Debe ser implementado por las clases derivadas para definir
     * como se procesan y analizan las mediciones del sensor.
     */
    virtual void procesarLectura() = 0;
    
    /**
     * @brief Metodo virtual puro para visualizacion de informacion
     * 
     * Debe ser implementado por las clases derivadas para mostrar
     * los detalles y datos del sensor.
     */
    virtual void imprimirInfo() const = 0;
    
    /**
     * @brief Obtiene el identificador del sensor
     * @return Puntero constante a la cadena del nombre
     */
    const char* getNombre() const {
        return nombre;
    }
};

#endif // SENSORBASE_H
