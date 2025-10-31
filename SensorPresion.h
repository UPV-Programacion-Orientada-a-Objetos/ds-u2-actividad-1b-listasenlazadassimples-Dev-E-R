/**
 * @file SensorPresion.h
 * @brief Implementacion de sensor barometrico para mediciones de presion
 * @author Sistema de Monitoreo
 * @version 1.0
 * @date 2024
 */

#ifndef SENSORPRESION_H
#define SENSORPRESION_H

#include "SensorBase.h"
#include "ListaSensor.h"
#include <iostream>
#include <iomanip>

/**
 * @class SensorPresion
 * @brief Clase especializada para dispositivos barometricos
 * 
 * Hereda de SensorBase e implementa la funcionalidad especifica
 * para sensores de presion. Almacena mediciones de tipo entero
 * y calcula la media aritmetica de los valores registrados.
 */
class SensorPresion : public SensorBase {
private:
    ListaSensor<int>* registroMediciones;  ///< Coleccion de mediciones de presion
    
public:
    /**
     * @brief Constructor parametrizado
     * @param identificador Codigo unico del sensor (por defecto "PRES-000")
     * 
     * Inicializa el sensor barometrico y crea una lista para almacenar mediciones.
     */
    SensorPresion(const char* identificador = "PRES-000") : SensorBase(identificador) {
        registroMediciones = new ListaSensor<int>();
        std::cout << "[Dispositivo Barometrico] Inicializado: " << nombre << std::endl;
    }
    
    /**
     * @brief Destructor especializado
     * 
     * Libera la memoria ocupada por el historial de mediciones.
     */
    ~SensorPresion() override {
        std::cout << "[Finalizacion " << nombre << "]" << std::endl;
        delete registroMediciones;
    }
    
    /**
     * @brief Incorpora una nueva medicion al registro
     * @param medida Valor de presion en Pascales
     * 
     * Agrega una nueva lectura de presion a la lista de mediciones.
     */
    void agregarLectura(int medida) {
        registroMediciones->insertarAlFinal(medida);
        std::cout << "[Dato] Valor entero " << medida << " almacenado" << std::endl;
    }
    
    /**
     * @brief Implementacion del metodo abstracto de procesamiento
     * 
     * Calcula y muestra la media aritmetica de las presiones registradas.
     * Si no hay mediciones disponibles, informa al usuario.
     */
    void procesarLectura() override {
        if (registroMediciones->estaVacia()) {
            std::cout << "[Dispositivo Barometrico] Registro vacio, sin datos para analizar" << std::endl;
            return;
        }
        
        // Calcular media aritmetica de las mediciones
        int acumulador = 0;
        int cantidadDatos = 0;
        
        registroMediciones->iterar([&acumulador, &cantidadDatos](int medida) {
            acumulador += medida;
            cantidadDatos++;
        });
        
        double mediaCalculada = static_cast<double>(acumulador) / cantidadDatos;
        std::cout << "[Dispositivo Barometrico] Media aritmetica: " 
                  << std::fixed << std::setprecision(2) << mediaCalculada << std::endl;
    }
    
    /**
     * @brief Implementacion del metodo abstracto de visualizacion
     * 
     * Muestra informacion detallada del sensor incluyendo su tipo,
     * identificador, cantidad de mediciones y el historial completo
     * de presiones registradas.
     */
    void imprimirInfo() const override {
        std::cout << "\n>>> Detalles del Dispositivo <<<" << std::endl;
        std::cout << "Categoria: Sensor Barometrico" << std::endl;
        std::cout << "Identificador: " << nombre << std::endl;
        std::cout << "Mediciones registradas: " << registroMediciones->getTamanio() << std::endl;
        
        if (!registroMediciones->estaVacia()) {
            std::cout << "Conjunto de datos: ";
            registroMediciones->iterar([](int medida) {
                std::cout << medida << " Pascales ";
            });
            std::cout << std::endl;
        }
        std::cout << "================================\n" << std::endl;
    }
    
    /**
     * @brief Accede al registro de mediciones
     * @return Puntero a la lista de mediciones de presion
     * 
     * Permite acceso directo al historial para operaciones avanzadas.
     */
    ListaSensor<int>* getHistorial() {
        return registroMediciones;
    }
};

#endif // SENSORPRESION_H
